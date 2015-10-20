#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T13:13:24
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = stereo_correlation
TEMPLATE = app

INCLUDEPATH += ../thirdparty/include

SOURCES += main.cpp\
imagecamera2d.cpp \
stereocorrelation.cpp \
qleftimagecanvaswidget.cpp \
qrightimagecanvaswidget.cpp \
qimagecanvaswidget.cpp

HEADERS  += common.h \
imagecamera2d.h \
stereocorrelation.h \
qleftimagecanvaswidget.h \
qrightimagecanvaswidget.h \
qimagecanvaswidget.h \
    metatype_includes.h

FORMS    += stereocorrelation.ui
