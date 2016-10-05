#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <list>
#include <string>
#include "cameraimage.h"
/**
 * @brief The CameraCalibration class handles the process of determining the fundamental matrix for a pair of cameras.
 * The CameraCalibration class takes a list of filenames, then uses the images in these files for stereo camera calibration.
 * The end result is a generated Stereo::Hardware::CameraRelationship that encapsulates a calibrated 3d camera.
 * To be clear, the CameraRelationship contains the OpenCV call stereoRectify, but _this_ class contains the OpenCV
 * call of stereoCalibrate (actually, an independent calibration for each camera individually, then using those to estimate
 * the parameters for stereoCalibrate). This is because we will save the results of CameraCalibration in a file to be
 * distributed with the software (i.e. the calibration process can create a library of common stereo camera calibrations),
 * but stereoRectify's results may or may not be saved to file (I haven't decided at this point).
 */

namespace Stereo
{
    namespace Algo
    {
        using namespace std;

        class CameraCalibration
        {
        public:

            typedef list<string> CalibrationImageFilenames;

            /**
             * @brief CameraCalibration constructor with list of preloaded images
             */
            CameraCalibration(CalibrationImageFilenames calibrationImages);

            /**
             * @brief processChessboardImages iterates through each image and attempts to use findChessboardCorners on the image.
             * Note that the CameraCalibration class is not interactive with the user, so ambiguity in the image, which is typically
             * resolved through asking the user if the results apper correct with drawChessboardCorners, which has to happen
             * somewhere else - in a gui app, it could be interactively; in a command-line app, it could be out-of-process with
             * saved images in a directory or some such.
             */
            vector<cv::Point2f> processChessboardImages();

        private:
            CalibrationImageFilenames m_calibrationImageLocations;
        };
    }
}
#endif // CAMERACALIBRATION_H
