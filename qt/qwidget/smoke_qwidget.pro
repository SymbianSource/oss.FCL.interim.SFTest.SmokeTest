load(qttest_p4)
SOURCES  += smoketest_qwidget.cpp
RESOURCES     = qwidget.qrc

contains(QT_CONFIG, qt3support): QT += qt3support
contains(QT_CONFIG, qt3support):DEFINES+=QT_HAS_QT3SUPPORT

aix-g++*:QMAKE_CXXFLAGS+=-fpermissive

CONFIG += x11inc

symbian  {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    LIBS += -leikcore -lcone -leikcoctl
}

!wince*:!symbian:win32: LIBS += -luser32 -lgdi32
