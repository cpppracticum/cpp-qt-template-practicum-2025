########
MOCK_LIB=../../../../mocks_library
########

QT += testlib
QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += qt warn_on depend_includepath testcase

CONFIG += c++17

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
        test_functions.cpp \
        $$SUBMISSION_PATH/mainwindow.cpp \
        $$SUBMISSION_PATH/setting.cpp


HEADERS += \
        utils.h \
        actions_logger.h \
        $$MOCK_LIB/prac/QInputDialog \
        $$SUBMISSION_PATH/functions.h \
        $$SUBMISSION_PATH/mainwindow.h \
        $$SUBMISSION_PATH/setting.h

FORMS += \
        $$SUBMISSION_PATH/mainwindow.ui \
        $$SUBMISSION_PATH/setting.ui
