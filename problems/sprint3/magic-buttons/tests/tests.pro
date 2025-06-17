########
MOCK_LIB=../../../../mocks_library
########

QT += testlib
QT += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += qt warn_on depend_includepath testcase

CONFIG += c++17

SUBMISSION_PATH = ../submission

INCLUDEPATH += $$SUBMISSION_PATH

TEMPLATE = app

SOURCES += \
        tests.cpp \
        $$SUBMISSION_PATH/functions.cpp \
        $$SUBMISSION_PATH/mainwindow.cpp \
        $$SUBMISSION_PATH/setting.cpp


HEADERS += \
        utils.h \
        actions_logger.h \
        $$SUBMISSION_PATH/functions.h \
        $$SUBMISSION_PATH/mainwindow.h \
        $$SUBMISSION_PATH/setting.h

FORMS += \
        $$SUBMISSION_PATH/mainwindow.ui \
        $$SUBMISSION_PATH/setting.ui
