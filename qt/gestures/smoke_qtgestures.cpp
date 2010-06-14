/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtTest/QtTest>
#include "../shared/util.h"

#include <qevent.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qgesture.h>
#include <qgesturerecognizer.h>
#include <qgraphicsitem.h>
#include <qgraphicsview.h>

#include <qdebug.h>

//TESTED_CLASS=
//TESTED_FILES=

static QPointF mapToGlobal(const QPointF &pt, QGraphicsItem *item, QGraphicsView *view)
{
    return view->mapToGlobal(view->mapFromScene(item->mapToScene(pt)));
}

class CustomGesture : public QGesture
{
    Q_OBJECT
public:
    static Qt::GestureType GestureType;

    CustomGesture(QObject *parent = 0)
        : QGesture(parent), serial(0)
    {
    }

    int serial;

    static const int SerialMaybeThreshold;
    static const int SerialStartedThreshold;
    static const int SerialFinishedThreshold;
};
Qt::GestureType CustomGesture::GestureType = Qt::CustomGesture;
const int CustomGesture::SerialMaybeThreshold = 1;
const int CustomGesture::SerialStartedThreshold = 3;
const int CustomGesture::SerialFinishedThreshold = 6;

class CustomEvent : public QEvent
{
public:
    static int EventType;

    CustomEvent(int serial_ = 0)
        : QEvent(QEvent::Type(CustomEvent::EventType)),
          serial(serial_), hasHotSpot(false)
    {
    }

    int serial;
    QPointF hotSpot;
    bool hasHotSpot;
};
int CustomEvent::EventType = 0;

class CustomGestureRecognizer : public QGestureRecognizer
{
public:
    static bool ConsumeEvents;

    CustomGestureRecognizer()
    {
        if (!CustomEvent::EventType)
            CustomEvent::EventType = QEvent::registerEventType();
    }

    QGesture* create(QObject *)
    {
        return new CustomGesture;
    }

    QGestureRecognizer::Result recognize(QGesture *state, QObject*, QEvent *event)
    {
        if (event->type() == CustomEvent::EventType) {
            QGestureRecognizer::Result result = 0;
            if (CustomGestureRecognizer::ConsumeEvents)
                result |= QGestureRecognizer::ConsumeEventHint;
            CustomGesture *g = static_cast<CustomGesture*>(state);
            CustomEvent *e = static_cast<CustomEvent*>(event);
            g->serial = e->serial;
            if (e->hasHotSpot)
                g->setHotSpot(e->hotSpot);
            if (g->serial >= CustomGesture::SerialFinishedThreshold)
                result |= QGestureRecognizer::FinishGesture;
            else if (g->serial >= CustomGesture::SerialStartedThreshold)
                result |= QGestureRecognizer::TriggerGesture;
            else if (g->serial >= CustomGesture::SerialMaybeThreshold)
                result |= QGestureRecognizer::MayBeGesture;
            else
                result = QGestureRecognizer::CancelGesture;
            return result;
        }
        return QGestureRecognizer::Ignore;
    }

    void reset(QGesture *state)
    {
        CustomGesture *g = static_cast<CustomGesture *>(state);
        g->serial = 0;
        QGestureRecognizer::reset(state);
    }
};
bool CustomGestureRecognizer::ConsumeEvents = false;

// same as CustomGestureRecognizer but triggers early without the maybe state
class CustomContinuousGestureRecognizer : public QGestureRecognizer
{
public:
    CustomContinuousGestureRecognizer()
    {
        if (!CustomEvent::EventType)
            CustomEvent::EventType = QEvent::registerEventType();
    }

    QGesture* create(QObject *)
    {
        return new CustomGesture;
    }

    QGestureRecognizer::Result recognize(QGesture *state, QObject*, QEvent *event)
    {
        if (event->type() == CustomEvent::EventType) {
            QGestureRecognizer::Result result = QGestureRecognizer::ConsumeEventHint;
            CustomGesture *g = static_cast<CustomGesture *>(state);
            CustomEvent *e = static_cast<CustomEvent *>(event);
            g->serial = e->serial;
            if (e->hasHotSpot)
                g->setHotSpot(e->hotSpot);
            if (g->serial >= CustomGesture::SerialFinishedThreshold)
                result |= QGestureRecognizer::FinishGesture;
            else if (g->serial >= CustomGesture::SerialMaybeThreshold)
                result |= QGestureRecognizer::TriggerGesture;
            else
                result = QGestureRecognizer::CancelGesture;
            return result;
        }
        return QGestureRecognizer::Ignore;
    }

    void reset(QGesture *state)
    {
        CustomGesture *g = static_cast<CustomGesture *>(state);
        g->serial = 0;
        QGestureRecognizer::reset(state);
    }
};

class GestureWidget : public QWidget
{
    Q_OBJECT
public:
    GestureWidget(const char *name = 0, QWidget *parent = 0)
        : QWidget(parent)
    {
        if (name)
            setObjectName(QLatin1String(name));
        reset();
        acceptGestureOverride = false;
    }
    void reset()
    {
        customEventsReceived = 0;
        gestureEventsReceived = 0;
        gestureOverrideEventsReceived = 0;
        events.clear();
        overrideEvents.clear();
        ignoredGestures.clear();
    }

    int customEventsReceived;
    int gestureEventsReceived;
    int gestureOverrideEventsReceived;
    struct Events
    {
        QList<Qt::GestureType> all;
        QList<Qt::GestureType> started;
        QList<Qt::GestureType> updated;
        QList<Qt::GestureType> finished;
        QList<Qt::GestureType> canceled;

        void clear()
        {
            all.clear();
            started.clear();
            updated.clear();
            finished.clear();
            canceled.clear();
        }
    } events, overrideEvents;

    bool acceptGestureOverride;
    QSet<Qt::GestureType> ignoredGestures;

protected:
    bool event(QEvent *event)
    {
        Events *eventsPtr = 0;
        if (event->type() == QEvent::Gesture) {
            QGestureEvent *e = static_cast<QGestureEvent*>(event);
            ++gestureEventsReceived;
            eventsPtr = &events;
            foreach(Qt::GestureType type, ignoredGestures)
                e->ignore(e->gesture(type));
        } else if (event->type() == QEvent::GestureOverride) {
            ++gestureOverrideEventsReceived;
            eventsPtr = &overrideEvents;
            if (acceptGestureOverride)
                event->accept();
        }
        if (eventsPtr) {
            QGestureEvent *e = static_cast<QGestureEvent*>(event);
            QList<QGesture*> gestures = e->gestures();
            foreach(QGesture *g, gestures) {
                eventsPtr->all << g->gestureType();
                switch(g->state()) {
                case Qt::GestureStarted:
                    eventsPtr->started << g->gestureType();
                    break;
                case Qt::GestureUpdated:
                    eventsPtr->updated << g->gestureType();
                    break;
                case Qt::GestureFinished:
                    eventsPtr->finished << g->gestureType();
                    break;
                case Qt::GestureCanceled:
                    eventsPtr->canceled << g->gestureType();
                    break;
                default:
                    Q_ASSERT(false);
                }
            }
        } else if (event->type() == CustomEvent::EventType) {
            ++customEventsReceived;
        } else {
            return QWidget::event(event);
        }
        return true;
    }
};

// TODO rename to sendGestureSequence
static void sendCustomGesture(CustomEvent *event, QObject *object, QGraphicsScene *scene = 0)
{
    for (int i = CustomGesture::SerialMaybeThreshold;
         i <= CustomGesture::SerialFinishedThreshold; ++i) {
        event->serial = i;
        if (scene)
            scene->sendEvent(qobject_cast<QGraphicsObject *>(object), event);
        else
            QApplication::sendEvent(object, event);
    }
}

class tst_Gestures : public QObject
{
Q_OBJECT

public:
    tst_Gestures();
    virtual ~tst_Gestures();

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void customGesture();
};

tst_Gestures::tst_Gestures()
{
}

tst_Gestures::~tst_Gestures()
{
}

void tst_Gestures::initTestCase()
{
    CustomGesture::GestureType = QGestureRecognizer::registerRecognizer(new CustomGestureRecognizer);
    QVERIFY(CustomGesture::GestureType != Qt::GestureType(0));
    QVERIFY(CustomGesture::GestureType != Qt::CustomGesture);
}

void tst_Gestures::cleanupTestCase()
{
    QGestureRecognizer::unregisterRecognizer(CustomGesture::GestureType);
}

void tst_Gestures::init()
{
}

void tst_Gestures::cleanup()
{
}

void tst_Gestures::customGesture()
{
    GestureWidget widget;
    widget.grabGesture(CustomGesture::GestureType, Qt::DontStartGestureOnChildren);
    CustomEvent event;
    sendCustomGesture(&event, &widget);

    static const int TotalGestureEventsCount = CustomGesture::SerialFinishedThreshold - CustomGesture::SerialStartedThreshold + 1;
    static const int TotalCustomEventsCount = CustomGesture::SerialFinishedThreshold - CustomGesture::SerialMaybeThreshold + 1;
    QCOMPARE(widget.customEventsReceived, TotalCustomEventsCount);
    QCOMPARE(widget.gestureEventsReceived, TotalGestureEventsCount);
    QCOMPARE(widget.gestureOverrideEventsReceived, 0);
    QCOMPARE(widget.events.all.size(), TotalGestureEventsCount);
    for(int i = 0; i < widget.events.all.size(); ++i)
        QCOMPARE(widget.events.all.at(i), CustomGesture::GestureType);
    QCOMPARE(widget.events.started.size(), 1);
    QCOMPARE(widget.events.updated.size(), TotalGestureEventsCount - 2);
    QCOMPARE(widget.events.finished.size(), 1);
    QCOMPARE(widget.events.canceled.size(), 0);
}

static const QColor InstanceColors[] = {
    Qt::blue, Qt::red, Qt::green, Qt::gray, Qt::yellow
};

class GestureItem : public QGraphicsObject
{
    static int InstanceCount;

public:
    GestureItem(const char *name = 0)
    {
        instanceNumber = InstanceCount++;
        if (name)
            setObjectName(QLatin1String(name));
        size = QRectF(0, 0, 100, 100);
        customEventsReceived = 0;
        gestureEventsReceived = 0;
        gestureOverrideEventsReceived = 0;
        events.clear();
        overrideEvents.clear();
        acceptGestureOverride = false;
    }
    ~GestureItem()
    {
        --InstanceCount;
    }

    int customEventsReceived;
    int gestureEventsReceived;
    int gestureOverrideEventsReceived;
    struct Events
    {
        QList<Qt::GestureType> all;
        QList<Qt::GestureType> started;
        QList<Qt::GestureType> updated;
        QList<Qt::GestureType> finished;
        QList<Qt::GestureType> canceled;

        void clear()
        {
            all.clear();
            started.clear();
            updated.clear();
            finished.clear();
            canceled.clear();
        }
    } events, overrideEvents;

    bool acceptGestureOverride;
    QSet<Qt::GestureType> ignoredGestures;

    QRectF size;
    int instanceNumber;

    void reset()
    {
        customEventsReceived = 0;
        gestureEventsReceived = 0;
        gestureOverrideEventsReceived = 0;
        events.clear();
        overrideEvents.clear();
        ignoredGestures.clear();
    }

protected:
    QRectF boundingRect() const
    {
        return size;
    }
    void paint(QPainter *p, const QStyleOptionGraphicsItem *, QWidget *)
    {
        QColor color = InstanceColors[instanceNumber % (sizeof(InstanceColors)/sizeof(InstanceColors[0]))];
        p->fillRect(boundingRect(), color);
    }

    bool event(QEvent *event)
    {
        Events *eventsPtr = 0;
        if (event->type() == QEvent::Gesture) {
            ++gestureEventsReceived;
            eventsPtr = &events;
            QGestureEvent *e = static_cast<QGestureEvent *>(event);
            foreach(Qt::GestureType type, ignoredGestures)
                e->ignore(e->gesture(type));
        } else if (event->type() == QEvent::GestureOverride) {
            ++gestureOverrideEventsReceived;
            eventsPtr = &overrideEvents;
            if (acceptGestureOverride)
                event->accept();
        }
        if (eventsPtr) {
            QGestureEvent *e = static_cast<QGestureEvent*>(event);
            QList<QGesture*> gestures = e->gestures();
            foreach(QGesture *g, gestures) {
                eventsPtr->all << g->gestureType();
                switch(g->state()) {
                case Qt::GestureStarted:
                    eventsPtr->started << g->gestureType();
                    break;
                case Qt::GestureUpdated:
                    eventsPtr->updated << g->gestureType();
                    break;
                case Qt::GestureFinished:
                    eventsPtr->finished << g->gestureType();
                    break;
                case Qt::GestureCanceled:
                    eventsPtr->canceled << g->gestureType();
                    break;
                default:
                    Q_ASSERT(false);
                }
            }
        } else if (event->type() == CustomEvent::EventType) {
            ++customEventsReceived;
        } else {
            return QGraphicsObject::event(event);
        }
        return true;
    }
};
int GestureItem::InstanceCount = 0;

QTEST_MAIN(tst_Gestures)
#include "smoke_qtgestures.moc"
