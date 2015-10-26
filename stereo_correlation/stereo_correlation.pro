#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T13:13:24
#
#-------------------------------------------------

QT       += core gui widgets

QT_CONFIG -= no-pkg-config

TARGET = stereo_correlation
TEMPLATE = app

# Use homebrew's ImageMagick++ libraries. In future, perhaps build and distribute alongside.
PKG_CONFIG = PKG_CONFIG_PATH=/usr/local/lib/pkgconfig /usr/local/bin/pkg-config
CONFIG += link_pkgconfig
PKGCONFIG = ImageMagick++

INCLUDEPATH += ../thirdparty/include

SOURCES += main.cpp\
imagecamera2d.cpp \
stereocorrelation.cpp \
qleftimagecanvaswidget.cpp \
qrightimagecanvaswidget.cpp \
qimagecanvaswidget.cpp \
    strcon.cpp \
    metatyperegistration.cpp \
    reticle.cpp

HEADERS  += common.h \
imagecamera2d.h \
stereocorrelation.h \
qleftimagecanvaswidget.h \
qrightimagecanvaswidget.h \
qimagecanvaswidget.h \
    strcon.h \
    metatyperegistration.h \
    reticle.h

FORMS    += stereocorrelation.ui

# Copies the given files to the destination directory
defineTest(copyToDestdir) {
    files = $$1

    for(FILE, files) {
        DDIR = $$OUT_PWD/stereo_correlation.app/Contents/Resources

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY $$PWD/$$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}

copyToDestdir(assets/reticle.png)
