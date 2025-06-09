########
MOCK_LIB=../../../../mocks_library
########

QT += testlib
QT += gui multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += qt warn_on depend_includepath testcase

CONFIG += c++20

SUBMISSION_PATH = ../submission

isEmpty(MOCK_LIB) {
    MOCK_LIB=/tmp/practicum-qt-mocks
}

INCLUDEPATH += $$SUBMISSION_PATH
INCLUDEPATH += $$MOCK_LIB


TEMPLATE = app

SOURCES += \
        tests.cpp \
        $$SUBMISSION_PATH/mainwindow.cpp \
        player.cpp

HEADERS += \
        utils.h \
        $$SUBMISSION_PATH/mainwindow.h \
        $$SUBMISSION_PATH/player.h \
        $$SUBMISSION_PATH/signal-processing.h

FORMS += \
        $$SUBMISSION_PATH/mainwindow.ui
