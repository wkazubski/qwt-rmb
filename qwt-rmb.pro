#-------------------------------------------------
#
# Project created by QtCreator 2014-03-07T16:06:03
#
#-------------------------------------------------

QT       -= gui

TARGET = qwt-rmb
TEMPLATE = lib

DEFINES += QWTRMB_LIBRARY

SOURCES += \
    qwt_picker2.cpp \
    qwt_picker_machine2.cpp \
    qwt_plot_picker2.cpp

HEADERS +=\
    qwt_picker2.h \
    qwt_picker_machine2.h \
    qwt_plot_picker2.h

unix {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }

    target.path = $$PREFIX/lib



    headers.path = $$PREFIX/include/qwt
    headers.files = $$HEADERS

    INSTALLS += target headers
}

unix:CONFIG += qwt
unix:INCLUDEPATH += /usr/include/qwt /usr/include/Qt
unix:LIBS += -lqwt
