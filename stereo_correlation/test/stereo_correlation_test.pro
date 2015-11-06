#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T13:13:24
#
#-------------------------------------------------

QT       += core gui widgets

QT_CONFIG -= no-pkg-config

CONFIG += c++11

TARGET = stereo_correlation
TEMPLATE = app

# Use homebrew's ImageMagick++ libraries. In future, perhaps build and distribute alongside.
PKG_CONFIG = PKG_CONFIG_PATH=/usr/local/lib/pkgconfig /usr/local/bin/pkg-config
CONFIG += link_pkgconfig
PKGCONFIG = ImageMagick++

INCLUDEPATH += ../../thirdparty/include \
../../thirdparty/include/jsoncpp/dist\


# Now set the locations of the source files
INCLUDEPATH += ..
VPATH += ..

# And our non-system libs
LIBS += -L"$$PWD/../../thirdparty/lib"
LIBS += -lgtest -lgtest_main

SOURCES += jsoncpp.cpp\
imagecamera2d.cpp \
stereocorrelation.cpp \
widgets/qleftimagecanvaswidget.cpp \
widgets/qrightimagecanvaswidget.cpp \
widgets/qimagecanvaswidget.cpp \
strcon.cpp \
metatyperegistration.cpp \
reticle.cpp \
image_pipeline/imagepipelinestepbase.cpp \
image_pipeline/saveimage.cpp \
imageprocessingview.cpp \
image_pipeline/imagepipeline.cpp \
image_pipeline/openimage.cpp \
image_pipeline/guiimagepipelinebuilder.cpp \
model/workspace.cpp \
model/coordinate.cpp \
    widgets/qcomputationaldagwidget.cpp \
    roundededgeorthographictextablebox.cpp \
    distancefieldglfont.cpp \
    qtdistancefieldfonttexturegenerator.cpp\
all_tests.cpp # Start with the tests now.


HEADERS  += common.h \
imagecamera2d.h \
stereocorrelation.h \
widgets/qleftimagecanvaswidget.h \
widgets/qrightimagecanvaswidget.h \
widgets/qimagecanvaswidget.h \
strcon.h \
metatyperegistration.h \
reticle.h \
image_pipeline/imagepipelinestepbase.h \
image_pipeline/saveimage.h \
imageprocessingview.h \
image_pipeline/imagepipeline.h \
image_pipeline/openimage.h \
image_pipeline/guiimagepipelinebuilder.h \
model/workspace.h \
model/coordinate.h \
    widgets/qcomputationaldagwidget.h \
    roundededgeorthographictextablebox.h \
    distancefieldglfont.h \
    qtdistancefieldfonttexturegenerator.h \
    strcon_test.h

FORMS    += forms/stereocorrelation.ui \
forms/imageprocessingview.ui

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

copyToDestdir(../assets/reticle.png)