#include "model/stereo_hardware/camerarelationship.h"
#include <opencv2/calib3d.hpp>

namespace Stereo
{
    namespace Hardware
    {
        using namespace cv;

        CameraRelationship::CameraRelationship(Camera::const_ptr left, Camera::const_ptr right):
            m_left(left), m_right(right),
            m_rotationLeftToRight(Mat::eye(3, 3, CV_64F)), m_translationLeftToRight(Mat::zeros(3, 1, CV_64F)),
            m_imageSize(), m_camerasCalibrated(false),
            m_camerasRectified(false)
        {

        }

        CameraRelationship::CameraRelationship(Camera::const_ptr left, Camera::const_ptr right, int intraocularDistance, Size imageSize):
            m_left(left), m_right(right),
            m_rotationLeftToRight(Mat::eye(3, 3, CV_64F)), m_translationLeftToRight(Mat::zeros(3, 1, CV_64F)),
            m_imageSize(imageSize), m_camerasCalibrated(false),
            m_camerasRectified(false)


        {
            /* Assumption: our cameras are not rotated and are translated horizontally by intraocularDistance.
             * This allows us to construct the rotation (no rotation) and translation (x-shift) values.
             *
             * This is most commonly used with reference data, which has been idealized for a "perfect pinhole
             * stereo camera" model. This is true of the data from [the Middlebury site](http://vision.middlebury.edu/stereo/data/2014/)
             */

            /* This would be the correct operation, but the identity matrix, which means "no rotation", happens
             * to coincide with what we use to default construct the matrix, so we're there already */
            //m_rotationLeftToRight = Mat::eye(3, 3, CV_64F);
            m_translationLeftToRight.at<double>(0, 0) = (double) intraocularDistance;
            m_camerasCalibrated = true;
            // We can go ahead and do this now. After this call, we have complete info about the relationship, so this constructor can be used for const.
            calculateRectificationMatrix();
        }

        void CameraRelationship::calculateRectificationMatrix()
        {
            //Q is wrong here. That means that some input is wrong here.
            stereoRectify(m_left->cameraMatrix(), m_left->distortionMatrix(),
                          m_right->cameraMatrix(), m_right->distortionMatrix(),
                          m_imageSize,
                          m_rotationLeftToRight, m_translationLeftToRight,
                          m_rectificationTransformLeft, m_rectificationTransformRight,
                          m_projectionLeft, m_projectionRight,
                          m_depthMappingMatrix);
            m_camerasRectified = true;
        }

        string CameraRelationship::to_s(Util::ToStringable::Verbosity verbosity) const
        {
            // For now, all same verbosity.
            Util::ToStringable::Verbosity verb = Util::ToStringable::low;
            switch(verb)
            {
            case(Util::ToStringable::low):
                return "Some string";
            }
        }

    }
}

