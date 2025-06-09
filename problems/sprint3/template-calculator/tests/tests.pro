########
MOCK_LIB=../../../../mocks_library
########

QT += testlib
QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += qt warn_on depend_includepath testcase

CONFIG += c++20

SUBMISSION_PATH = ../submission

DEFINES += "PRAC_TESTS"

isEmpty(MOCK_LIB) {
    MOCK_LIB=/tmp/practicum-qt-mocks
}

INCLUDEPATH += $$SUBMISSION_PATH
INCLUDEPATH += $$MOCK_LIB

TEMPLATE = app

SOURCES += \
        tests.cpp \
        $$SUBMISSION_PATH/mainwindow.cpp \

HEADERS += \
        utils.h \
        calculator_tests.h \
        $$SUBMISSION_PATH/mainwindow.h \
        $$SUBMISSION_PATH/calculator.h \
        $$SUBMISSION_PATH/controller.h \
        $$SUBMISSION_PATH/enums.h \
        $$SUBMISSION_PATH/pow.h \
        $$SUBMISSION_PATH/rational.h


FORMS += \
        $$SUBMISSION_PATH/mainwindow.ui
