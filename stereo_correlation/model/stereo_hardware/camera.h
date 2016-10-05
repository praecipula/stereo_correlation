#ifndef CAMERA_HARDWARE_H
#define CAMERA_HARDWARE_H

#include <memory>
#include <opencv2/core/core.hpp>
#include "algorithm/stereo/cameraimage.h"
#include "algorithm/stereo/undistortedimage.h"

namespace Stereo
{
    namespace Hardware
    {
        using namespace cv;
        using namespace std;
        /**
         * Camera represents one of the pair of real-world stereo cameras.
         *
         * Each of these cameras has a set of physical properties
         * that can be discovered through OpenCV and/or read from disk, and these properties are used in rectifying and understanding
         * the underlying 3d scene.
         *
         * In OpenCV terms, this class acts as a mediator composed of the camera matrix and its distortion parameters, wrapped in a
         * neater package that includes features like serialization and encapsulation for our processing needs. It is responsible
         * for either loading these parameters from a file or doing a calibration with calibrateCamera. It is _not_ responsible
         * for stereoCalibrate - that is @see CameraRelationship
         */
        class Camera
        {
        public:
            // typedefs
            typedef std::shared_ptr<Camera> ptr;
            typedef std::shared_ptr<const Camera> const_ptr;

            // For these, see http://docs.opencv.org/2.4/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html, top of the page.
            typedef Matx<float, 3, 3> CameraMatrix;
            typedef Matx<float, 1, 8> DistortionMatrix;

            /**
             * When the parameters are known programmatically from OpenCV matrices.
             * @param cameraMatrix a 3x3 matrix representing the camera as a pinhole camera
             * @param distortionMatrix a single-column matrix (vector) of 6 distortion params
             */
            Camera(const CameraMatrix& cameraMatrix, const DistortionMatrix& distortionMatrix);
            /**
             * When calibrating via chessboard images.
             * Pass in the set of images, and we'll use OpenCV's tools to run the camera calibration and calculate its
             * camera and distortion matrix.
             * @param chessboardImagesSet a set of views of the same calibration image (chessboard) to use for calibration
             */
            Camera(shared_ptr< vector<Algo::CameraImage> > chessboardImagesSet);

            /**
             * @brief Run a camera calibration on a set of chessboard images.
             * This will overwrite the camera calibration parameters inside of this class, so it is non-const.
             */
            void calibrate(shared_ptr< vector<Algo::CameraImage> > chessboardImagesSet);

            /**
             * Undistort a distorted source image.
             * The undistorted image will be as if this camera had no distortion, i.e. the camera matrix is the same,
             * but the distortion matrix is the 1x6 vector of 0's
             *
             * Note that this is single-camera undistortion, suitable for e.g. displaying a single image from a single camera.
             * It is not useful for creating processing-worthy remaps of the source image for sake of calculating dispartiy.
             * for that, see @see CameraRelationship::recifyImagePair
             *
             * @param sourceImage the source image
             * @return a new image that is the undistorted copy of the source image
             */
            Algo::UndistortedImage undistortImage(const Algo::CameraImage& sourceImage) const;

            // Accessors

            const CameraMatrix cameraMatrix() const {return m_cameraMatrix;}
            const DistortionMatrix distortionMatrix() const {return m_distortionMatrix;}

        private:

            CameraMatrix m_cameraMatrix;
            DistortionMatrix m_distortionMatrix;

        };
    }
}

#endif
