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
public:
    // We know that we are taking one image of the given calibration pattern, so let's
    // DRY up some of the code
    static Algo::CameraCalibration::ptr createCalibrationImageCase(string filename) {
        Algo::CameraCalibration::CalibrationImageFilenames filenames;
        filenames.push_back(filename);
        return make_shared<Algo::CameraCalibration>(filenames, 8, 8, 38);
    }

 protected:


};

TEST_F(CameraCalibrationTest, FindsPerfectChessboardImageCorners) {
    string chessboardFilename = StrCon(QCoreApplication::applicationDirPath() + "/../Resources/camera_calibration_test/perfect.png").to_s();
    Algo::CameraCalibration::ptr calibration = CameraCalibrationTest::createCalibrationImageCase(chessboardFilename);
    vector<cv::Point2f> chessboardCorners = calibration->processChessboardImages();
    EXPECT_EQ(49, chessboardCorners.size());
}

TEST_F(CameraCalibrationTest, FindsNearlyFullChessboardImageCorners) {

    string chessboardFilename = StrCon(QCoreApplication::applicationDirPath() + "/../Resources/camera_calibration_test/nearly_full_cleaned.jpg").to_s();
    Algo::CameraCalibration::ptr calibration = CameraCalibrationTest::createCalibrationImageCase(chessboardFilename);
    vector<cv::Point2f> chessboardCorners = calibration->processChessboardImages();
    EXPECT_EQ(49, chessboardCorners.size());
}

TEST_F(CameraCalibrationTest, DoesNotFindCloseChessboardImageCorners) {
    // This is not clean enough to pass (it's a counter-example image that helps highlight where we fail).
    string chessboardFilename = StrCon(QCoreApplication::applicationDirPath() + "/../Resources/camera_calibration_test/close_cleaned.jpg").to_s();
    Algo::CameraCalibration::ptr calibration = CameraCalibrationTest::createCalibrationImageCase(chessboardFilename);
    vector<cv::Point2f> chessboardCorners = calibration->processChessboardImages();
    EXPECT_NE(49, chessboardCorners.size());
}


#endif // CAMERA_CALIBRATION_TEST
