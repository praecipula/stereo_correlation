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



# This is only a test.
DEFINES += "STEREO_TEST=1"

# Now set the locations of the source files
INCLUDEPATH += ..
VPATH += ..


# Here's where we move them.
# Deployment for MACOSX libs to bundle.
# Note that this does _not_ run macdeployqt, but rather fixes up the
# libs in the thirdparty dir.
macx {


    # And our non-system libs.
    # We pre-populate them at the build dir, so that we link to their already-copied location.
    BFRAMEWORK_DIR = $$OUT_PWD/stereo_correlation.app/Contents/Frameworks
    DIST_DIR=$$OUT_PWD/../../dist/MACOSX

    prebuildcopytarget.commands += mkdir -p $$quote($$BFRAMEWORK_DIR) $$escape_expand(\\n\\t)
    prebuildcopytarget.target = this

    # Simplify things to DRY up.
    defineTest(preBuildCopy) {
        FRAMEWORKS = $$1
        for(FRAMEWORK, FRAMEWORKS) {
            FRAMEWORKDEST = $$BFRAMEWORK_DIR/$$FRAMEWORK
            !exists($$FRAMEWORKDEST) {
                prebuildcopytarget.commands += $$QMAKE_COPY_DIR $$quote($$DIST_DIR/$$FRAMEWORK) $$quote($$BFRAMEWORK_DIR) $$escape_expand(\\n\\t)
            }
        }
        export(prebuildcopytarget.commands)
    }

    preBuildCopy(Googletest.framework)
    preBuildCopy(Boost.framework)
    preBuildCopy(ThreadingBuildingBlocks.framework)
    preBuildCopy(Log4CPlus.framework)
    preBuildCopy(Blosc.framework)
    preBuildCopy(OpenCV.framework)
    preBuildCopy(OpenVDB.framework)
    preBuildCopy(IlmBase.framework)

    QMAKE_EXTRA_TARGETS += prebuildcopytarget
    # Predepend on this target
    PRE_TARGETDEPS += $$prebuildcopytarget.target

}

INCLUDEPATH += ../../thirdparty/include \
../../thirdparty/include/jsoncpp/dist\

QMAKE_CXXFLAGS += -isystem ../../thirdparty/include \
../../thirdparty/include/jsoncpp/dist
QMAKE_CXXFLAGS += -I$$quote($$BFRAMEWORK_DIR/OpenCV.framework/Headers)

# Add an rpath for framework libs
QMAKE_LFLAGS += '-Wl,-rpath,@loader_path/../Frameworks'

LIBS += -F$$quote($$BFRAMEWORK_DIR/)
LIBS += -framework Googletest -framework Googletest,main \
-framework Boost -framework Boost,system -framework Boost,filesystem \
-framework ThreadingBuildingBlocks \
-framework Log4CPlus \
-framework Blosc \
-framework OpenCV \
-framework OpenVDB \
-framework IlmBase


SOURCES += jsoncpp.cpp\
imagecamera2d.cpp \
stereocorrelation.cpp \
widgets/qleftimagecanvaswidget.cpp \
widgets/qrightimagecanvaswidget.cpp \
widgets/qimagecanvaswidget.cpp \
widgets/qstereomeshwidget.cpp \
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
model/pointcloud.cpp \
algorithm/stereo/cameracalibration.cpp \
algorithm/stereo/projectpoints.cpp \
algorithm/stereo/disparity.cpp \
algorithm/stereo/imagebase.cpp \
model/stereo_hardware/camera.cpp \
model/stereo_hardware/camerarelationship.cpp \
widgets/qcomputationaldagwidget.cpp \
roundededgeorthographictextablebox.cpp \
distancefieldglfont.cpp \
qtdistancefieldfonttexturegenerator.cpp\
all_tests.cpp \ # Start with the tests now.
../utility/tostringable.cpp


HEADERS  += common.h \
imagecamera2d.h \
stereocorrelation.h \
widgets/qleftimagecanvaswidget.h \
widgets/qrightimagecanvaswidget.h \
widgets/qimagecanvaswidget.h \
widgets/qstereomeshwidget.h \
strcon.h \
metatyperegistration.h \
reticle.h \
image_pipeline/imagepipelinestepbase.h \
image_pipeline/saveimage.h \
imageprocessingview.h \
image_pipeline/imagepipeline.h \
image_pipeline/openimage.h \
image_pipeline/guiimagepipelinebuilder.h \
algorithm/stereo/cameracalibration.h \
model/workspace.h \
model/coordinate.h \
model/pointcloud.h \
    widgets/qcomputationaldagwidget.h \
    roundededgeorthographictextablebox.h \
    distancefieldglfont.h \
    qtdistancefieldfonttexturegenerator.h \
    strcon_test.h \
    qtdistancefieldfonttexturegenerator_test.h \
    qstereomeshwidget_test.h \
    stereomesh_test.h \
    stereo_point_projection_test.h \
    ../model/stereo_hardware/camera.h \
    ../model/stereo_hardware/camerarelationship.h \
    ../algorithm/stereo/semiglobalblockmatching.h \
    ../algorithm/stereo/cameraimage.h \
    ../algorithm/stereo/disparity.h \
    ../algorithm/stereo/imagebase.h \
    ../algorithm/stereo/projectpoints.h \
    ../algorithm/stereo/undistortedimage.h \
    ../model/pointcloud.h \
    ../utility/tostringable.h \
    camera_calibration_test.h \
    image_pipeline_test.h \
    openimage_test.h

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

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$PWD/$$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }
    export(QMAKE_POST_LINK)
}

defineTest(copyDirToDestdir) {
    files = $$1

    for(FILE, files) {
        DDIR = $$OUT_PWD/stereo_correlation.app/Contents/Resources

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$PWD/$$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }
    export(QMAKE_POST_LINK)
}

# Original assets
copyToDestdir(../assets/reticle.png)

# Test assets
copyDirToDestdir(./assets/qtdistancefieldfonttexturegenerator_test)
copyDirToDestdir(./assets/stereomesh_test)
copyDirToDestdir(./assets/project_points_test)
copyDirToDestdir(./assets/camera_calibration_test)
