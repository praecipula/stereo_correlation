#include "gtest/gtest.h"
#include <QApplication>
#include "common.h"
#include "strcon_test.h"
//#include "qtdistancefieldfonttexturegenerator_test.h"
//#include "stereomesh_test.h"
//#include "stereo_point_projection_test.h"
#include "qstereomeshwidget_test.h"
#include "camera_calibration_test.h"
#include "image_pipeline_test.h"


int main(int argc, char **argv) {
    // Some tests need a running QApplication
    QApplication app(argc, argv);
    QApplication::processEvents(); // this should allow `QApplication` to complete its initialization
    ::testing::InitGoogleTest(&argc, argv);
    Stereo::initialize();
    return RUN_ALL_TESTS();
}
