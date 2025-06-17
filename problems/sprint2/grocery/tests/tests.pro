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
        utils.cpp \
        $$SUBMISSION_PATH/additemsnotification.cpp \
        $$SUBMISSION_PATH/cartwindow.cpp \
        $$SUBMISSION_PATH/selector.cpp

HEADERS += \
        utils.h \
        $$SUBMISSION_PATH/additemsnotification.h \
        $$SUBMISSION_PATH/cartwindow.h \
        $$SUBMISSION_PATH/clickablelabel.h \
        $$SUBMISSION_PATH/definitions.h \
        $$SUBMISSION_PATH/selector.h

FORMS += \
        $$SUBMISSION_PATH/additemsnotification.ui \
        $$SUBMISSION_PATH/cartwindow.ui \
        $$SUBMISSION_PATH/selector.ui
