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


#if defined(QT3_SUPPORT)
#include <q3hbox.h>
#include <q3textedit.h>
#endif
#include <qboxlayout.h>
#include <qapplication.h>
#include <qbitmap.h>
#include <qdebug.h>
#include <qeventloop.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qpushbutton.h>
#include <qstyle.h>
#include <qwidget.h>
#include <qwindowsstyle.h>
#include <qinputcontext.h>
#include <qdesktopwidget.h>
#include <private/qwidget_p.h>
#include <private/qapplication_p.h>
#include <qcalendarwidget.h>
#include <qmainwindow.h>
#include <qdockwidget.h>
#include <qtoolbar.h>
#include <QtGui/qpaintengine.h>
#include <private/qbackingstore_p.h>

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsProxyWidget>

#include "../shared/util.h"


#ifdef Q_WS_S60
#include <avkon.hrh>                // EEikStatusPaneUidTitle
#include <akntitle.h>               // CAknTitlePane
#include <akncontext.h>             // CAknContextPane
#include <eikspane.h>               // CEikStatusPane
#include <eikbtgpc.h>               // CEikButtonGroupContainer
#endif

#ifdef Q_WS_QWS
# include <qscreen_qws.h>
#endif

// I *MUST* have QtTest afterwards or this test won't work with newer headers
#if defined(Q_WS_MAC)
# include <private/qt_mac_p.h>
#undef verify
#include "tst_qwidget_mac_helpers.h"  // Abstract the ObjC stuff out so not everyone must run an ObjC++ compile.
#endif

#include <QtTest/QtTest>

#if defined(Q_WS_WIN)
#  include <qt_windows.h>
#  if !defined(Q_OS_WINCE)
#define Q_CHECK_PAINTEVENTS \
    if (::SwitchDesktop(::GetThreadDesktop(::GetCurrentThreadId())) == 0) \
        QSKIP("desktop is not visible, this test would fail", SkipSingle);
#  else
#    define Q_CHECK_PAINTEVENTS
#  endif
#elif defined(Q_WS_X11)
#  include <private/qt_x11_p.h>
#  include <qx11info_x11.h>
#elif defined(Q_WS_QWS)
# include <qwindowsystem_qws.h>
#endif

#if !defined(Q_WS_WIN)
#define Q_CHECK_PAINTEVENTS
#endif

#if defined(Bool)
#undef Bool
#endif

// Will try to wait for the condition while allowing event processing
// for a maximum of 2 seconds.
#define WAIT_FOR_CONDITION(expr, expected) \
    do { \
        const int step = 100; \
        for (int i = 0; i < 2000 && expr != expected; i+=step) { \
            QTest::qWait(step); \
        } \
    } while(0)

//TESTED_CLASS=
//TESTED_FILES=

class tst_QWidget : public QObject
{
    Q_OBJECT

public:
    tst_QWidget();
    virtual ~tst_QWidget();


public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
private slots:
    void showFullScreen();
	
private:
    bool ensureScreenSize(int width, int height);
    QWidget *testWidget;
};

tst_QWidget::tst_QWidget()
{
    QFont font;
    font.setBold(true);
    font.setPointSize(42);
    qApp->setFont(font, "QPropagationTestWidget");

    QPalette palette;
    palette.setColor(QPalette::ToolTipBase, QColor(12, 13, 14));
    palette.setColor(QPalette::Text, QColor(21, 22, 23));
    qApp->setPalette(palette, "QPropagationTestWidget");

    testWidget = 0;
}

tst_QWidget::~tst_QWidget()
{
}

class BezierViewer : public QWidget {
public:
    BezierViewer( QWidget* parent=0, const char* name=0 );
    void paintEvent( QPaintEvent* );
    void setPoints( const QPolygonF& poly );
private:
    QPolygonF points;

};

void tst_QWidget::initTestCase()
{
  // Create the test class
    testWidget = new BezierViewer( 0, "testObject");
    testWidget->resize(200,200);
#ifdef QT3_SUPPORT
    qApp->setMainWidget(testWidget);
#endif
    testWidget->show();
    QTest::qWaitForWindowShown(testWidget);
    QTest::qWait(50);
}

void tst_QWidget::cleanupTestCase()
{
    delete testWidget;
    testWidget = 0;
}


void tst_QWidget::init()
{
// TODO: Add initialization code here.
// This will be executed immediately before each test is run.
    testWidget->setFont(QFont());
    testWidget->setPalette(QPalette());
}

void tst_QWidget::cleanup()
{
}



// Helper class...

BezierViewer::BezierViewer( QWidget* parent, const char* name )
	: QWidget( parent )
{
    setObjectName(name);
    QPalette pal;
    pal.setColor(backgroundRole(), Qt::white);
    setPalette(pal);
}


void BezierViewer::setPoints( const QPolygonF& a )
{
    points = a;
}

#include "private/qbezier_p.h"
void BezierViewer::paintEvent( QPaintEvent* )
{
    if ( points.size() != 4 ) {
#if defined(QT_CHECK_RANGE)
	qWarning( "QPolygon::bezier: The array must have 4 control points" );
#endif
	return;
    }

    // Calculate Bezier curve
    QPolygonF bezier = QBezier::fromPoints(points.at(0),points.at(1),points.at(2),points.at(3)).toPolygon();

    QPainter painter( this );

    // Calculate scale to fit in window
    QRectF br = bezier.boundingRect() | points.boundingRect();
    QRectF pr = rect();
    int scl = qMax( qMin(pr.width()/br.width(), pr.height()/br.height()), qreal(1.) );
    int border = scl-1;

    // Scale Bezier curve vertices
    for ( QPolygonF::Iterator it = bezier.begin(); it != bezier.end(); ++it ) {
	it->setX( (it->x()-br.x()) * scl + border );
	it->setY( (it->y()-br.y()) * scl + border );
    }

    // Draw grid
    painter.setPen( Qt::lightGray );
	int i;
	for ( i = border; i <= pr.width(); i += scl ) {
		painter.drawLine( i, 0, i, pr.height() );
    }
    for ( int j = border; j <= pr.height(); j += scl ) {
	painter.drawLine( 0, j, pr.width(), j );
    }

    // Write number of vertices
    painter.setPen( Qt::red );
    painter.setFont( QFont("Helvetica", 14, QFont::DemiBold, TRUE ) );
    QString caption;
    caption.setNum( bezier.size() );
    caption += QString::fromLatin1( " vertices" );
    painter.drawText( 10, pr.height()-10, caption );

    // Draw Bezier curve
    painter.setPen( Qt::black );
    painter.drawPolyline( bezier );

    // Scale and draw control points
    painter.setPen( Qt::darkGreen );
    for ( QPolygonF::Iterator p1 = points.begin(); p1 != points.end(); ++p1 ) {
	int x = (p1->x()-br.x()) * scl + border;
	int y = (p1->y()-br.y()) * scl + border;
	painter.drawLine( x-4, y-4, x+4, y+4 );
	painter.drawLine( x+4, y-4, x-4, y+4 );
    }

    // Draw vertices
    painter.setPen( Qt::red );
    painter.setBrush( Qt::red );
    for ( QPolygonF::Iterator p2 = bezier.begin(); p2 != bezier.end(); ++p2 )
	painter.drawEllipse( p2->x()-1, p2->y()-1, 3, 3 );
}

void tst_QWidget::showFullScreen()
{
    QWidget plain;
    QHBoxLayout *layout;
    QWidget layouted;
    QLineEdit le;
    QLineEdit le2;
    QLineEdit le3;
    layout = new QHBoxLayout;

    layout->addWidget(&le);
    layout->addWidget(&le2);
    layout->addWidget(&le3);

    layouted.setLayout(layout);

    plain.showFullScreen();
    QVERIFY(plain.windowState() & Qt::WindowFullScreen);

    plain.showNormal();
    QVERIFY(!(plain.windowState() & Qt::WindowFullScreen));

    layouted.showFullScreen();
    QVERIFY(layouted.windowState() & Qt::WindowFullScreen);

    layouted.showNormal();
    QVERIFY(!(layouted.windowState() & Qt::WindowFullScreen));

#if !defined(Q_WS_QWS) && !defined(Q_OS_WINCE) && !defined (Q_WS_S60)
//embedded may choose a different size to fit on the screen.
    QCOMPARE(layouted.size(), layouted.sizeHint());
#endif

    layouted.showFullScreen();
    QVERIFY(layouted.isFullScreen());
    QVERIFY(layouted.isVisible());

    layouted.hide();
    QVERIFY(layouted.isFullScreen());
    QVERIFY(!layouted.isVisible());

    layouted.showFullScreen();
    QVERIFY(layouted.isFullScreen());
    QVERIFY(layouted.isVisible());

    layouted.showMinimized();
    QVERIFY(layouted.isMinimized());
    QVERIFY(layouted.isFullScreen());

    layouted.showFullScreen();
    QVERIFY(!layouted.isMinimized());
    QVERIFY(layouted.isFullScreen());
    QVERIFY(layouted.isVisible());

    layouted.showMinimized();
    QVERIFY(layouted.isMinimized());
    QVERIFY(layouted.isFullScreen());

    layouted.showFullScreen();
    QVERIFY(!layouted.isMinimized());
    QVERIFY(layouted.isFullScreen());
    QVERIFY(layouted.isVisible());

    {
        QWidget frame;
        QWidget widget(&frame);
        widget.showFullScreen();
        QVERIFY(widget.isFullScreen());
    }

#ifdef QT3_SUPPORT
#if !defined(Q_WS_QWS)
//embedded respects max/min sizes by design -- maybe wrong design, but that's the way it is now.
    {
        Q3HBox box;
        QWidget widget(&box);
        widget.setMinimumSize(500, 500);
        box.showFullScreen();
        QVERIFY(box.isFullScreen());
    }

    {
        Q3HBox box;
        QWidget widget(&box);
        widget.setMaximumSize(500, 500);

        box.showFullScreen();
        QVERIFY(box.isFullScreen());
    }
#endif
#endif // QT3_SUPPORT
}

QTEST_MAIN(tst_QWidget)
#include "smoke_qwidget.moc"
