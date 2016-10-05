#ifndef STEREO_POINT_PROJECTION_TEST
#define STEREO_POINT_PROJECTION_TEST


#include "model/pointcloud.h"
#include "strcon.h"
#include <iostream>
#include <QCoreApplication>
#include <QFile>
#include "model/stereo_hardware/camera.h"
#include "model/stereo_hardware/camerarelationship.h"
#include "algorithm/stereo/disparity.h"
#include "algorithm/stereo/projectpoints.h"
#include "model/pointcloud.h"

using namespace Stereo;
using namespace std;
using namespace cv;


class FriendlyProjectPoints : public Algo::ProjectPoints
{
public:
    FriendlyProjectPoints() :
        Algo::ProjectPoints() {}

};

class StereoPointProjectionTest : public ::testing::Test {
 protected:
    FriendlyProjectPoints m_projector;
    Algo::Disparity::ptr m_disparity;

    StereoPointProjectionTest() :
        m_projector(), m_disparity()
    {

        string disparityFilename = StrCon(QCoreApplication::applicationDirPath() + "/../Resources/project_points_test/Adirondack-perfect/disp0.pfm").to_s();
        m_disparity = Algo::Disparity::load_pfm(disparityFilename);

        // This is known a priori by reading calib.txt in the assets for this test.
        Hardware::Camera::CameraMatrix leftCameraMat(4161.221, 0, 1445.577,
                                                     0, 4161.221, 984.686,
                                                     0, 0, 1);
        Hardware::Camera::CameraMatrix rightCameraMat(4161.221, 0, 1654.636,
                                                      0, 4161.221, 984.686,
                                                      0, 0, 1);
        // Assumption: the images we receive are pre-calibrated.
        Hardware::Camera::DistortionMatrix noDistortion(0, 0, 0, 0, 0, 0, 0, 0);

        // Construct the left and right camera pointers
        Hardware::Camera::const_ptr leftCamera(new Hardware::Camera(leftCameraMat, noDistortion));
        Hardware::Camera::const_ptr rightCamera(new Hardware::Camera(rightCameraMat, noDistortion));
        // Use those to construct the CameraRelationship
        Hardware::CameraRelationship::const_ptr relationship(new Hardware::CameraRelationship(leftCamera, rightCamera, 176.252, m_disparity->size()));
        // Almost there. set the relationship for m_projector
        m_projector.setCameraRelationship(relationship);

        // Open the disparity image for use in the test.

    }

};

TEST_F(StereoPointProjectionTest, CreatesPointCloud) {

    PointCloud cloud = m_projector(m_disparity);

}


#endif // STEREO_POINT_PROJECTION_TEST
