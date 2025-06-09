########
MOCK_LIB=../../../../mocks_library
########

QT += testlib
QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += qt warn_on depend_includepath testcase

CONFIG += c++20
DEFINES += "PRAC_TESTS"

SUBMISSION_PATH = ../submission

isEmpty(MOCK_LIB) {
    MOCK_LIB=/tmp/practicum-qt-mocks
}

INCLUDEPATH += $$SUBMISSION_PATH
INCLUDEPATH += $$MOCK_LIB

TEMPLATE = app

SOURCES +=  \
    tests.cpp \
    view.cpp

HEADERS += \
    $$SUBMISSION_PATH/controller.h \
    view.h \
    model.h

