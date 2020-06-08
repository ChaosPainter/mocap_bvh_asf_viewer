#-------------------------------------------------
#
# Project created by QtCreator 2018-05-22T20:25:32
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += core widgets

greaterThan(QT_MAJOR_VERSION, 4) : QT+=widgets printsupport #needed for qcustomplot to work

TARGET = new
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        qcustomplot.cpp \
        data.cpp \
        qwidgetgl.cpp \
        vertex.cpp \
    transform3d.cpp \
    input.cpp \
    camera3d.cpp

HEADERS += \
        mainwindow.h \
        qcustomplot.h \
        data.h \
        qwidgetgl.h \
        vertex.h \
    transform3d.h \
    input.h \
    camera3d.h

FORMS += \
        mainwindow.ui

win32:LIBS += libopengl32
DISTFILES +=

RESOURCES += \
    resources.qrc


