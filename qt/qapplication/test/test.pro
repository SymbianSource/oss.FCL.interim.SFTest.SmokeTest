load(qttest_p4)

SOURCES += ../smoketest_qapplication.cpp
TARGET = ../smoketest_qapplication

someTest.sources = test.pro
someTest.path = test
DEPLOYMENT = additional deploy someTest windowIcon
LIBS += -lcone -lavkon

symbian {
    TARGET = ../../release/smoketest_qapplication
}


