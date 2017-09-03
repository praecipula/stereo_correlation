#ifndef CAMERA_CALIBRATION_TEST
#define CAMERA_CALIBRATION_TEST


#include "algorithm/stereo/cameracalibration.h"
#include <QCoreApplication>
#include <opencv2/core.hpp>

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
    vector<Algo::CameraCalibration::PointsAndFilename> chessboardCorners = calibration->processChessboardImages();
    EXPECT_EQ(49, chessboardCorners.front().first.size());
}

TEST_F(CameraCalibrationTest, FindsNearlyFullCleanedChessboardImageCorners) {
    // "Cleaned" here means "shrunk by half". (I don't know why the following test - the original - must go down to 1/16 to get the points; it's
    // missing one from 1/2 and 1/4.
    string chessboardFilename = StrCon(QCoreApplication::applicationDirPath() + "/../Resources/camera_calibration_test/nearly_full_cleaned.jpg").to_s();
    Algo::CameraCalibration::ptr calibration = CameraCalibrationTest::createCalibrationImageCase(chessboardFilename);
    vector<Algo::CameraCalibration::PointsAndFilename> chessboardCorners = calibration->processChessboardImages();
    EXPECT_EQ(49, chessboardCorners.front().first.size());
}

TEST_F(CameraCalibrationTest, FindsNearlyFullChessboardImageCorners) {

    string chessboardFilename = StrCon(QCoreApplication::applicationDirPath() + "/../Resources/camera_calibration_test/nearly_full.jpg").to_s();
    Algo::CameraCalibration::ptr calibration = CameraCalibrationTest::createCalibrationImageCase(chessboardFilename);
    vector<Algo::CameraCalibration::PointsAndFilename> chessboardCorners = calibration->processChessboardImages();
    EXPECT_EQ(49, chessboardCorners.front().first.size());
}

TEST_F(CameraCalibrationTest, DoesNotFindCloseChessboardImageCorners) {
    // This is not clean enough to pass (it's a counter-example image that helps highlight where we fail).
    string chessboardFilename = StrCon(QCoreApplication::applicationDirPath() + "/../Resources/camera_calibration_test/close_cleaned.jpg").to_s();
    Algo::CameraCalibration::ptr calibration = CameraCalibrationTest::createCalibrationImageCase(chessboardFilename);
    vector<Algo::CameraCalibration::PointsAndFilename> chessboardCorners = calibration->processChessboardImages();
    EXPECT_NE(49, chessboardCorners.front().first.size());
}

TEST_F(CameraCalibrationTest, FindsCalibrationResult) {
    // These all tested OK, with the exception of 308172, which I'm leaving in to test.
    cv::setBreakOnError(true);
    // These all work, but don't all have the right orientation.
//    const char* filename_fragments[] = {
//            "G0048093.JPG", "G0078103.JPG", "G0118113.JPG", "G0178133.JPG", "G0278163.JPG",
//            "G0048094.JPG", "G0088104.JPG", "G0118114.JPG", "G0188134.JPG", "G0288164.JPG",
//            "G0088105.JPG", "G0118115.JPG", "G0158125.JPG", "G0188135.JPG", "G0258155.JPG", "G0288165.JPG",
//            "G0088106.JPG", "G0158126.JPG", "G0188136.JPG", "G0228146.JPG", "G0258156.JPG", "G0288166.JPG",
//            "G0158127.JPG", "G0198137.JPG", "G0228147.JPG", "G0258157.JPG",
//            "G0068098.JPG", "G0168128.JPG", "G0198138.JPG", "G0228148.JPG",
//            "G0038089.JPG", "G0068099.JPG", "G0138119.JPG", "G0168129.JPG", "G0198139.JPG",
//            "G0038090.JPG", "G0068100.JPG", "G0108110.JPG", "G0138120.JPG", "G0168130.JPG",
//            "G0038091.JPG", "G0078101.JPG", "G0108111.JPG", "G0138121.JPG", "G0178131.JPG", "G0278161.JPG",
//            "G0048092.JPG", "G0078102.JPG", "G0108112.JPG", "G0178132.JPG", "G0278162.JPG"//, "G0308172.JPG" failed to handle correctly a zero case
//            };

    // These are all ltr, top-bottom oriented
    const char* filename_fragments[] = {
            "G0048093.JPG", "G0178133.JPG", "G0278163.JPG"//,
//            "G0048094.JPG", "G0288164.JPG",
//            "G0258155.JPG",
//            "G0228146.JPG", "G0258156.JPG",
//            "G0228147.JPG", "G0258157.JPG",
//            "G0168128.JPG", "G0228148.JPG",
//            "G0038089.JPG",
//            "G0038090.JPG", "G0108110.JPG", "G0168130.JPG",
//            "G0038091.JPG", "G0108111.JPG", "G0178131.JPG",
//            "G0048092.JPG", "G0108112.JPG", "G0278162.JPG"//, "G0308172.JPG"
            };
     Algo::CameraCalibration::CalibrationImageFilenames filenames;
     for (const char* fragment : filename_fragments) {
        filenames.push_back(StrCon(QCoreApplication::applicationDirPath() + "/../Resources/camera_calibration_test/" + fragment).to_s());
     }
     Algo::CameraCalibration calibration(filenames, 8, 8, 38);
     Algo::CameraCalibration::CalibrationResults::ptr results = calibration.runCalibration();
     results->serialize();
}


#endif // CAMERA_CALIBRATION_TEST
