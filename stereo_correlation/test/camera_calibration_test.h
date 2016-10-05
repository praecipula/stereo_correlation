#ifndef CAMERA_CALIBRATION_TEST
#define CAMERA_CALIBRATION_TEST


#include "algorithm/stereo/cameracalibration.h"
#include <QCoreApplication>

using namespace Stereo;
using namespace std;
using namespace cv;


class FriendlyCameraCalibration : public Algo::CameraCalibration
{
public:
    using CameraCalibration::processChessboardImages;

};

class CameraCalibrationTest : public ::testing::Test {
 protected:


};

TEST_F(CameraCalibrationTest, ProcessesChessboardImage) {


}


#endif // CAMERA_CALIBRATION_TEST
