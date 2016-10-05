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

INCLUDEPATH += ../thirdparty/include \
../thirdparty/include/jsoncpp/dist

QMAKE_CXXFLAGS += -isystem ../thirdparty/include \
../thirdparty/include/jsoncpp/dist

SOURCES += jsoncpp.cpp\
main.cpp\
imagecamera2d.cpp \
stereocorrelation.cpp \
widgets/qleftimagecanvaswidget.cpp \
widgets/qrightimagecanvaswidget.cpp \
widgets/qimagecanvaswidget.cpp \
widgets/qstereomeshwidget.cpp \
model/coordinate.cpp \
model/workspace.cpp \
strcon.cpp \
metatyperegistration.cpp \
reticle.cpp \
image_pipeline/imagepipelinestepbase.cpp \
image_pipeline/saveimage.cpp \
imageprocessingview.cpp \
image_pipeline/imagepipeline.cpp \
image_pipeline/openimage.cpp \
image_pipeline/guiimagepipelinebuilder.cpp \
widgets/qcomputationaldagwidget.cpp \
roundededgeorthographictextablebox.cpp \
distancefieldglfont.cpp \
qtdistancefieldfonttexturegenerator.cpp \
    model/pointcloud.cpp \
    algorithm/stereo/imagebase.cpp \
    algorithm/stereo/disparity.cpp \
    algorithm/stereo/projectpoints.cpp \
    model/stereo_hardware/camerarelationship.cpp \
    model/stereo_hardware/camera.cpp \
    utility/tostringable.cpp \
    algorithm/stereo/cameracalibration.cpp


HEADERS  += common.h \
imagecamera2d.h \
stereocorrelation.h \
widgets/qleftimagecanvaswidget.h \
widgets/qrightimagecanvaswidget.h \
widgets/qimagecanvaswidget.h \
widgets/qstereomeshwidget.h \
    model/coordinate.h \
    model/workspace.h \
strcon.h \
metatyperegistration.h \
reticle.h \
image_pipeline/imagepipelinestepbase.h \
image_pipeline/saveimage.h \
imageprocessingview.h \
image_pipeline/imagepipeline.h \
image_pipeline/openimage.h \
image_pipeline/guiimagepipelinebuilder.h \
widgets/qcomputationaldagwidget.h \
roundededgeorthographictextablebox.h \
distancefieldglfont.h \
qtdistancefieldfonttexturegenerator.h \
    model/stereo_hardware/camera.h \
    model/stereo_hardware/camerarelationship.h \
    algorithm/stereo/semiglobalblockmatching.h \
    algorithm/stereo/disparity.h \
    algorithm/stereo/cameraimage.h \
    algorithm/stereo/imagebase.h \
    algorithm/stereo/undistortedimage.h \
    algorithm/stereo/projectpoints.h \
    algorithm/stereo/pointcloud.h \
    model/pointcloud.h \
    utility/tostringable.h \
    algorithm/stereo/cameracalibration.h

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

copyToDestdir(assets/reticle.png)
