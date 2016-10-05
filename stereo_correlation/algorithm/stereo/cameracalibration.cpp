#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include "cameracalibration.h"
#include "common.h"

namespace Stereo
{
    namespace Algo
    {

        CameraCalibration::CameraCalibration(CalibrationImageFilenames calibrationImages) :
            m_calibrationImageLocations(calibrationImages)
        {

        }

        vector<cv::Point2f> CameraCalibration::processChessboardImages()
        {
            /* Implicit requirement: all chessboard images must be fully in frame,
             * and all chessboards are standard 8x8 boards.
             * We could likely relax this later, but it's good enough for now.
             */
            cv::Size patternsize(8, 6);
            // Desired: output array of x,y location of corners.
            vector<cv::Point2f> corners;
            for (string filename : m_calibrationImageLocations)
            {
                // Open the file, make grayscale.
                ImageBase::ptr pImage = ImageBase::load(filename);
                ImageBase::ptr pGray = pImage->grayscale_copy();

                // Pass it to findChessboardCorners, which does a fast check
                bool patternfound = cv::findChessboardCorners(*pGray, patternsize, corners,
                                                          CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
                                                          + CALIB_CB_FAST_CHECK);
                // If we found the pattern
                if(!patternfound)
                {
                    LOGW << "Could not find chessboard corners for " << filename;
                    continue;
                }

                // Do a fine-grained subpixel find for the corners.
                cv::cornerSubPix(*pGray, corners, Size(11, 11), Size(-1, -1),
                             TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
                // Draw and display the image
                cv::drawChessboardCorners(*pGray, patternsize, Mat(corners), patternfound);
            }
            // We should now have a number of corners detected.
            // Determine that there are enough corners.
            // Return the corners to be used for the camera calibration step.
            return corners;
        }
    }
}
