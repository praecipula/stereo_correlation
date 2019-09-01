#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include "cameracalibration.h"
#include "common.h"

namespace Stereo
{
    namespace Algo
    {
//
//        OrderableChessboardCorners::OrderableChessboardCorners(PointVector points,
//                                                               int chessboardSquaresWide,
//                                                               int chessboardSquaresHigh) :
//        m_originalVector(points), m_orderedVector(),
//          m_chessboardSquaresWide(chessboardSquaresWide), m_chessboardSquaresHigh(chessboardSquaresHigh)
//        {}
//
//        OrderableChessboardCorners::PointVector OrderableChessboardCorners::inOrder()
//        {
//            if (m_orderedVector.empty())
//            {
//                //first, order the points by y-coordinate
//            }
//            return m_orderedVector;
//        }
//
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

        vector<cv::Point3f> CameraCalibration::getChessboardObjectPoints() const
        {
            vector<cv::Point3f> chessboardCorners;
            for( int j = 0; j < m_chessboardSquaresHigh - 1; ++j )
                for( int i = 0; i < m_chessboardSquaresWide - 1; ++i )
                    chessboardCorners.push_back(Point3f(float( i*m_chessboardSquareMm ), float( j*m_chessboardSquareMm ), 0));
            return chessboardCorners;
        }

        vector<CameraCalibration::PointsAndFilename> CameraCalibration::processChessboardImages()
        {

            vector<PointsAndFilename> detectedImages;

            // In classic OpenCV confusing documentation,
            //"( patternSize = cvSize(points_per_row,points_per_colum) = cvSize(columns,rows) )."
            //But it's really the interior points, which is (column_squares - 1, row_squares - 1)
            cv::Size patternSize(m_chessboardSquaresWide - 1, m_chessboardSquaresHigh - 1);
            int expectedNumPoints = patternSize.width * patternSize.height;
            int imgno = 0;
            for (string filename : m_calibrationImageLocations)
            {
                LOGD << "Working with " << filename;
                // These are the discovered corners, and may be in the scaled image's dimensions.
                vector<cv::Point2f> corners;
                // Open the file, make grayscale.
                ImageBase::ptr pColor = ImageBase::load(filename);
                ImageBase::ptr pGray = pColor->grayscale_copy();

                pGray->save("/tmp/chessboard_orig.jpg");
                ImageBase::ptr scaled;
                int fraction = 1;
                bool patternFound;
                //The images are of a certain size, but the corner detection
                do
                {
                    scaled = pGray->rect_scaled_copy(1.0 / fraction);

                    std::stringstream filename_buf;
                    filename_buf << "/tmp/chessboard_orig_scaled_" << fraction << ".jpg";
                    scaled->save(filename_buf.str());

                    // Loop over scales (smaller scales are better, larger scales are more accurate) until we find the image
                    // Pass it to findChessboardCorners, which does a fast check
                    patternFound = cv::findChessboardCorners(*scaled, patternSize, corners,
                                                             CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_FAST_CHECK);
                    // If we found the pattern
                    if(!patternFound)
                    {
                        LOGI << "Could not find chessboard corners for " << filename << " at " << fraction << "(" << corners.size() << " of " << expectedNumPoints << " found)";
                        fraction = fraction * 2;
                    } else {
                        // Break out of the loop - this is success. (the loop is "never succeeds")
                        LOGI << "Found chessboard corners for " << filename << " at " << fraction << "(" << corners.size() << " of " << expectedNumPoints << " found)";
                        break;
                    }
                } while (scaled->size().width > 480);
                // Untransform the points according to the scale.
                vector<cv::Point2f> unscaledCorners;
                for (cv::Point2f& corner : corners) {
                    unscaledCorners.push_back(cv::Point2f(corner.x * fraction, corner.y * fraction));
                }
                if (patternFound)
                {
                    // Do a fine-grained subpixel find for the unscaled corners.
                    // Note: this is a bit different than the standard cornerSubPix arguments, because we want the window to scale
                    // with the accuracy from the original pixel find, which was perhaps hampered by scaling. So we use the _full_
                    // window size and divide it by 2, so Size(10, 10) * (fraction[1]/2.0) => 5, 5 half-window => 10, 10 window.
                    // Size(10, 10) * (fraction[2]/2.0) => 10, 10 half-window => 20, 20, covering the same area in the scaled-down
                    // version of the image (assuming that the error in the normal sized image from finding the scaled up one
                    // is constant - so the neighborhood of error will have scaled). Note that there's integer floor-ing at 1,
                    // so we never *actually* go smaller than the specified window here.
                    cv::cornerSubPix(*pGray, unscaledCorners, Size(10, 10) * fraction/2, Size(-1, -1),
                               TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));
                }
                // Draw and display the image. We do this even if we don't get a pattern, because it can sometimes draw some of the points.
                cv::drawChessboardCorners(*pColor, patternSize, Mat(unscaledCorners), patternFound);
                LOGI << filename << " is image no " << imgno;
                std::stringstream stream;
                stream << "/tmp/chessboard" << imgno << ".jpg";
                LOGD << "Saving to " << stream.str();
                pColor->save(stream.str());
                imgno++;
                detectedImages.push_back(make_pair(unscaledCorners, filename));
            } //end looping over each filename
            return detectedImages;
        }

        CameraCalibration::CalibrationResults::ptr CameraCalibration::runCalibration()
        {

            // Assume fisheye lens. This could be a parameter somewhere in the future.
            CameraCalibration::CalibrationResults::ptr results(new CameraCalibration::CalibrationResults());
            //TODO: stereoCalibrate for true epipolar calibration.
            vector<vector<cv::Point3f>> objectPoints;
            vector<CameraCalibration::PointsAndFilename> imageAndFiles = processChessboardImages();
            // Loop and put into a vector of point2f (strip out string)
            vector<vector<cv::Point2f>> imageCornerPoints;
            // Use this loop to both pull out just the points and to generate object points as well.
            for (CameraCalibration::PointsAndFilename pandf : imageAndFiles) {
              imageCornerPoints.push_back(pandf.first);
              objectPoints.push_back(getChessboardObjectPoints());
              results->rotationVectorsPerImage.push_back(Mat());
              results->translationVectorsPerImage.push_back(Mat());
            }
            // Open an image and get its size. Arbitrarily choose first image returned from the chessboard images.
            ImageBase::ptr pColor = ImageBase::load(imageAndFiles.front().second);
            // The data stored here are small enough that we don't care about making a shared pointer at this time.
            // Run it!
            try {
                double distortionError = cv::calibrateCamera(objectPoints, imageCornerPoints,  pColor->size(),
                                                             results->cameraMatrixK, results->distortionCoefficientsD,
                                                             results->rotationVectorsPerImage, results->translationVectorsPerImage,
                                                             CALIB_RATIONAL_MODEL);
                results->reprojectionError = distortionError;
                LOGI << results->cameraMatrixK;
                LOGI << results->distortionCoefficientsD;
                //LOGI << results->rotationVectorsPerImage;
                //LOGI << results->translationVectorsPerImage;
                LOGI << "Got results with reprojection error" << results->reprojectionError;
                int imgno = 0;
                for (CameraCalibration::PointsAndFilename pandf : imageAndFiles) {
                    ImageBase::ptr pColor = ImageBase::load(pandf.second);
                    Mat undistorted;
                    cv::undistort(*pColor, undistorted,
                                  results->cameraMatrixK, results->distortionCoefficientsD);
                    ImageBase::ptr output(new ImageBase(undistorted));
                    std::stringstream stream;
                    stream << "/tmp/undistort" << imgno << ".jpg";
                    output->save(stream.str());
                    imgno++;
                }
                LOGI << "Calibration complete";
                return results;

            } catch (const exception& e) {
                LOGE << "Got exception";
            }


        }

    }
}
