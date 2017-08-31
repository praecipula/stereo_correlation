#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include "cameracalibration.h"
#include "common.h"

namespace Stereo
{
    namespace Algo
    {

        CameraCalibration::CameraCalibration(CalibrationImageFilenames calibrationImages,
                                             int chessboardSquaresWide,
                                             int chessboardSquaresHigh,
                                             int chessboardSquareMm) :
            m_calibrationImageLocations(calibrationImages),
            m_chessboardSquaresWide(chessboardSquaresWide),
            m_chessboardSquaresHigh(chessboardSquaresHigh),
            m_chessboardSquareMm(chessboardSquareMm)
        {

        }

        vector<cv::Point2f> CameraCalibration::processChessboardImages()
        {
            /* Implicit requirement: all chessboard images must be fully in frame,
             * and all chessboards are standard 8x8 boards.
             * We could likely relax this later, but it's good enough for now.
             */

            // In classic OpenCV confusing documentation,
            //"( patternSize = cvSize(points_per_row,points_per_colum) = cvSize(columns,rows) )."
            //But it's really the interior points, which is (column_squares - 1, row_squares - 1)
            cv::Size patternSize(m_chessboardSquaresWide - 1, m_chessboardSquaresHigh - 1);
            int expectedNumPoints = patternSize.width * patternSize.height;
            // Desired: output array of x,y location of corners.
            // TODO: this should be an array of N(m_calibrationImageLocations), not a single one.
            vector<cv::Point2f> corners;
            for (string filename : m_calibrationImageLocations)
            {
                // Open the file, make grayscale.
                ImageBase::ptr pGray = ImageBase::loadGrayscale(filename);

                pGray->save("/tmp/chessboard_orig.jpg");
                ImageBase::ptr scaled;
                int fraction = 1;
                bool patternFound;
                //Arbitrary: we can't get good signal with a chessboard if the image is too small
                do
                {
                    scaled = pGray->rect_scaled_copy(1.0 / fraction);

                    std::stringstream filename_buf;
                    filename_buf << "/tmp/chessboard_orig_scaled_" << fraction << ".jpg";
                    scaled->save(filename_buf.str());

                    // Loop over scales (smaller scales are better, larger scales are more accurate) until we find the image
                    // Pass it to findChessboardCorners, which does a fast check
                    patternFound = cv::findChessboardCorners(*scaled, patternSize, corners,
                                                             CALIB_CB_ADAPTIVE_THRESH + CV_CALIB_CB_FAST_CHECK);
                    // If we found the pattern
                    if(!patternFound)
                    {
                        LOGD << "Could not find chessboard corners for " << filename << " at " << fraction << "(" << corners.size() << " of " << expectedNumPoints << " found)";
                        fraction = fraction * 2;
                    } else {
                        // Break out of the loop - this is success. (the loop is "never succeeds")
                        LOGI << "Found chessboard corners for " << filename << " at " << fraction << "(" << corners.size() << " of " << expectedNumPoints << " found)";
                        break;
                    }
                } while (scaled->size().width > 480);
                if (patternFound)
                {
                    // Do a fine-grained subpixel find for the corners.
                    cv::cornerSubPix(*pGray, corners, Size(11, 11), Size(-1, -1),
                               TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
                }
                // Draw and display the image
                cv::drawChessboardCorners(*pGray, patternSize, Mat(corners), patternFound);
                pGray->save("/tmp/chessboard.jpg");
            }
            // We should now have a number of corners detected.
            // Determine that there are enough corners.
            // Return the corners to be used for the camera calibration step.
            return corners;
        }
    }
}
