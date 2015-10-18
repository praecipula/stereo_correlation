#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T13:13:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = stereo_correlation
TEMPLATE = app

INCLUDEPATH += ../lib/headers
LIBS += "-L../lib/lib"

SOURCES += main.cpp\
        stereocorrelation.cpp \
    qleftimagecanvaswidget.cpp \
    qimagecanvaswidget.cpp

HEADERS  += stereocorrelation.h \
    qleftimagecanvaswidget.h \
    qimagecanvaswidget.h

FORMS    += stereocorrelation.ui
