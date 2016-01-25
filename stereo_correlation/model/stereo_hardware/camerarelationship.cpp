#ifndef CAMERA_RELATIONSHIP_H
#define CAMERA_RELATIONSHIP_H

#include <opencv2/core/core.hpp>
#include <memory>
#include "camera.h"
#include "algorithm/stereo/undistortedimage.h"

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
        class CameraRelationship
        {
        public:

            /**
             */
            CameraRelationship();


            /**
             * When constructing with two cameras in the left-right configuration - our standard case.
             *
             * OpenCV is capable of much more, but this assumption simplifies things quite a bit in the
             * interface of this class.
             *
             * @param left a pointer to the (const) left camera model
             * @param right a pointer to the (const) right camera model
             * @param intraocularDistance the distance between the centers of both cameras' lenses
             */
            CameraRelationship(Camera::const_ptr left, Camera::const_ptr right, int intraocularDistance);

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

            // Typedefs
            typedef shared_ptr<CameraRelationship> ptr;
            typedef shared_ptr<const CameraRelationship> const_ptr;

        };
    }
}

#endif // CAMERA_RELATIONSHIP_H

