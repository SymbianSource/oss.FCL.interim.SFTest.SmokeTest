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


//#define QT_TST_QAPP_DEBUG
#include <qdebug.h>

#include <QtTest/QtTest>

#include "qabstracteventdispatcher.h"
#include <QtGui>

#include "private/qapplication_p.h"
#include "private/qstylesheetstyle_p.h"
#ifdef Q_OS_WINCE
#include <windows.h>
#endif
#ifdef Q_OS_SYMBIAN
#include <aknenv.h>
#endif

//TESTED_CLASS=
//TESTED_FILES=

#if defined(Q_OS_SYMBIAN)
// In Symbian, the PluginsPath doesn't specify the only absolute path; just the dir that can be found on any drive
/*static void addExpectedSymbianPluginsPath(QStringList& expected)
{
    QString installPathPlugins = QDir::fromNativeSeparators(QLibraryInfo::location(QLibraryInfo::PluginsPath));
    QFileInfoList driveList = QDir::drives();
    QListIterator<QFileInfo> iter(driveList);
    while (iter.hasNext()) {
        QFileInfo testFi(iter.next().canonicalPath().append(installPathPlugins));
        if (testFi.exists())
            expected << testFi.canonicalFilePath();
    }
}
*/
#endif

class tst_QApplication : public QObject
{
Q_OBJECT

public:
    tst_QApplication();
    virtual ~tst_QApplication();

public slots:
    void init();
    void cleanup();
private slots:
    void setActiveWindow();
};

static  char *argv0;

tst_QApplication::tst_QApplication()
{
#ifdef Q_OS_WINCE
    // Clean up environment previously to launching test
    qputenv("QT_PLUGIN_PATH", QByteArray());
#endif
}

tst_QApplication::~tst_QApplication()
{

}

void tst_QApplication::init()
{
// TODO: Add initialization code here.
// This will be executed immediately before each test is run.
}

void tst_QApplication::cleanup()
{
// TODO: Add cleanup code here.
// This will be executed immediately after each test is run.
}

void tst_QApplication::setActiveWindow()
{
    int argc = 0;
    QApplication MyApp(argc, 0, QApplication::GuiServer);

    QWidget* w = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(w);

    QLineEdit* pb1 = new QLineEdit("Testbutton1", w);
    QLineEdit* pb2 = new QLineEdit("Test Line Edit", w);

    layout->addWidget(pb1);
    layout->addWidget(pb2);

    pb2->setFocus();
    pb2->setParent(0);
    delete pb2;

    w->show();
    QApplication::setActiveWindow(w); // needs this on twm (focus follows mouse)
    QVERIFY(pb1->hasFocus());
    delete w;
}

//QTEST_APPLESS_MAIN(tst_QApplication)
int main(int argc, char *argv[])
{
    tst_QApplication tc;
    argv0 = argv[0];
    return QTest::qExec(&tc, argc, argv);
}

#include "smoke_qapplication.moc"
