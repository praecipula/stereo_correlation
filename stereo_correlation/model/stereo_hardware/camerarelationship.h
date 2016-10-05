#ifndef CAMERA_RELATIONSHIP_H
#define CAMERA_RELATIONSHIP_H

#include <opencv2/core/core.hpp>
#include <memory>
#include "camera.h"
#include "algorithm/stereo/undistortedimage.h"
#include "utility/tostringable.h"

namespace Stereo
{
    namespace Hardware
    {
        using namespace cv;

        /**
         * A CameraRelationship describes the geometry between left and right images
         * In a sense, it encapsulates the state data and the functionality as provided by OpenCV's
         * stereoRectify function. It can, however, also be created manually if the parameters
         * are known.
         *
         * The workflow of getting a valid CameraRelationship is essentially to use the Camera's data
         * and run it through
         */
        class CameraRelationship :
                public Util::ToStringable
        {
        public:

            // Typedefs
            typedef shared_ptr<CameraRelationship> ptr;
            typedef shared_ptr<const CameraRelationship> const_ptr;

//            typedef Matx<int, 3, 3> RotationMatrix;
//            typedef Matx<int, 3, 1> TranslationVector;
//            typedef Matx<int, 3, 4> ProjectionMatrix;
//            typedef Matx<int, 4, 4> DisparityToDepthMatrix;

            /**
             */
            CameraRelationship();


            /**
             * When constructing with two cameras.
             *
             * The relationship between these cameras is not specified here, so a further call to
             * calibrate() is required to calculate the relationships (rotation and translation)
             * between cameras. calibrate() is called with an input set of data (measurements)
             * on the scene and infers the rotation and translation from this data. If this data
             * is known, i.e. if the images are pre-calibrated to an ideal stereo camera model,
             * see the other constructor.
             *
             * @param left a pointer to the (const) left camera model
             * @param right a pointer to the (const) right camera model
             */
            CameraRelationship(Camera::const_ptr left, Camera::const_ptr right);


            /**
             * Constructing with an a priori calibrated rotation and translation setup.
             *
             * This is useful with pre-calibrated data, where there is assumed to be no rotation between
             * image planes and a linear, x-axis translation between cameras
             */
            CameraRelationship(Camera::const_ptr left, Camera::const_ptr right, int horizontalIntraocularDistance, Size imageSize);

            /**
                stereoCalibrate
             */


            /**
             * Remap images taken by both cameras to be homographically related
             * Once the camera relationship has been established, we use this function in order to
             * take any new inputs (new, distorted images) and map them to be homographic - that
             * is, the epipolar lines are parallel and at the same Y-coordinate for each image.
             *
             * It is this pair of images to be used for @see Algo::SemiGlobalBlockMatching
             */
            // TODO: this return type should be a typedef or a first-class type for semantic purposes of "a stereo image pair"
            std::pair<Algo::UndistortedImage, Algo::UndistortedImage> rectifyImagePair(const Mat& left, const Mat& right);


            /**
             * Generate a reprojected image.
             *
             */

            /**
             * Calculate disparity handling matrices
             *
             * More or less cv::stereoRectify
             * http://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html#stereorectify
             * which caches the results in memory for faster future lookup.
             */
            void calculateRectificationMatrix();

            // Accessors
            const Mat depthMappingMatrix() const {return m_depthMappingMatrix;}
            /** Alias depthMappingMatrix to align with OpenCV's naming convention
             */
            const Mat Q() const {return depthMappingMatrix();}

            // ToStringable
            virtual string to_s(Verbosity verbosity) const;




        private:
            Camera::const_ptr m_left;
            Camera::const_ptr m_right;



            /* These are calculated with assumptions from the camera geometry */
            Mat m_rotationLeftToRight;
            Mat m_translationLeftToRight;
            Size m_imageSize;
            /* When the above are set, this is true */
            bool m_camerasCalibrated;

            /* cached versions of the return parameters of cv::stereoRectify.
             * Might as well lazily-evaluate and cache. */
            Mat m_rectificationTransformLeft;
            Mat m_rectificationTransformRight;
            Mat m_projectionLeft;
            Mat m_projectionRight;
            Mat m_depthMappingMatrix;
            bool m_camerasRectified;

        };
    }
}

#endif // CAMERA_RELATIONSHIP_H

