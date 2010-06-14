load(qttest_p4)

SOURCES += ../smoke_qapplication.cpp
TARGET = ../smoke_qapplication

someTest.sources = test.pro
someTest.path = test
DEPLOYMENT = additional deploy someTest windowIcon
LIBS += -lcone -lavkon

symbian {
    TARGET = ../../release/tst_smoke_qapplication
}


