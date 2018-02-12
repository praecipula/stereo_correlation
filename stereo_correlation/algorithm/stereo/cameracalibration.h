#ifndef CAMERACALIBRATION_H
#define CAMERACALIBRATION_H

#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
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
        namespace pt = boost::property_tree;

        /**
         * @brief Class to fix to iterate from row-major, top-left=>bottom-right order
         *
         * The problem here is that depending on the orientation of the chessboard, there is a particular
         * interior corner that can be used to start the returned point iteration. This means that instead of
         * always getting a top-left=>bottom right iteration, we get an arbitrary ordering.
         *
         * This method will ensure a top-left, bottom-right ordering for most cases.
         * The way it works is using the geometry of the chessboard: when the board is intended to be iterated
         * in row-major, column-major order, and we have the geometry of the chessboard, we simply sort it twice:
         * first, by y-coordinate - this will order the points into several "buckets" of coordinates, where the
         * size of the bucket is the width of the chessboard (for instance, an 8x8-square chessboard will create
         * 7 points on its first row).
         *
         * We then re-sort each row by its x-coordinate. This means that the first 7 elements, sorted, should
         * iterate left to right, then jump to the next row.
         *
         * @param points the randomly oriented points
         * @return an ordered vector of points in row-major, column-major (top-left, bottom-right) order
         */
//        class OrderableChessboardCorners
//        {
//        public:
//            typedef vector<cv::Point2f> PointVector;
//
//            OrderableChessboardCorners(PointVector points, int chessboardSquaresWide, int chessboardSquaresHigh);
//
//            PointVector inOrder();
//        private:
//            PointVector m_originalVector;
//            PointVector m_orderedVector;
//            int m_chessboardSquaresWide;
//            int m_chessboardSquaresHigh;
//        };


        class CameraCalibration
        {
        public:

            typedef shared_ptr<CameraCalibration> ptr;
            typedef list<string> CalibrationImageFilenames;
            typedef pair<vector<cv::Point2f>, string> PointsAndFilename;

            struct CalibrationResults
            {
                typedef shared_ptr<CalibrationResults> ptr;

                CalibrationResults():
                    cameraMatrixK(Mat::eye(3, 3, CV_64F)),
                    distortionCoefficientsD(Mat::zeros(8, 1, CV_64F)),
                    rotationVectorsPerImage(vector<Mat>()),
                    translationVectorsPerImage(vector<Mat>()),
                    reprojectionError(-1) {}

                Mat cameraMatrixK;
                Mat distortionCoefficientsD;
                vector<Mat> rotationVectorsPerImage;
                vector<Mat> translationVectorsPerImage;
                double reprojectionError;

                //TODO: move this into general serialization area for configs.
                template <typename MatrixType>
                struct MatrixSerializer
                {
                    MatrixSerializer(){}

                    pt::ptree serialize(const Mat& matrix) {
                        pt::ptree node;
                        node.put("rows", matrix.rows);
                        node.put("cols", matrix.cols);
                        pt::ptree data;
                        for(int r = 0; r < matrix.rows; ++r) {
                            pt::ptree rowNode;
                            Mat row = matrix.row(r);
                            for(int c = 0; c < matrix.cols; ++c){
                                pt::ptree valueNode;
                                valueNode.put("", matrix.at<MatrixType>(r, c));
                                rowNode.push_back(make_pair("", valueNode));
                            }
                            data.push_back(make_pair("", rowNode));
                        }
                        node.add_child("data", data);
                        return node;
                    }

                    Mat deserialize(const pt::ptree& node){
                        int rows = node.get<int>("rows");
                        int cols = node.get<int>("cols");
                        Mat mat = Mat(rows, cols, CV_64F);
                        pt::ptree data = node.get_child("data");
                        int r = 0;
                        BOOST_FOREACH(const pt::ptree::value_type& row, data.get_child("")) {
                            int c = 0;
                            BOOST_FOREACH(const pt::ptree::value_type& node, row.second.get_child("")) {
                                MatrixType value = node.second.get<MatrixType>("");
                                mat.at<MatrixType>(r, c) = value;
                                ++c;
                            }
                            ++r;
                        }
                        return mat;
                    }
                };

                void serialize(std::string filename)
                {
                    pt::ptree tree;
                    tree.put("serialization_version", "1.0");

                    pt::ptree calib;

                    MatrixSerializer<double> ser;
                    pt::ptree cameraMatrixNode = ser.serialize(cameraMatrixK);
                    calib.add_child("camera", cameraMatrixNode);
                    pt::ptree distortionCoefNode = ser.serialize(distortionCoefficientsD);
                    calib.add_child("distortion", distortionCoefNode);
                    calib.put("reprojection_error", reprojectionError);
                    // At this time, it's not clear that there's any value in adding in the
                    // translation and rotation vectors for the chessboards for future use.
                    // (In fact, the reprojection_error is less useful, but it can be
                    // used to evaluate by eye looking at the file how good the calibration
                    // seems to be.
                    tree.add_child("calibration", calib);
                    pt::write_json(filename, tree);
                }

                void deserialize(std::string filename) {
                    ifstream stream;
                    stream.open(filename, ifstream::in);
                    pt::ptree tree;
                    pt::read_json(stream, tree);

                    pt::ptree calib = tree.get_child("calibration");

                    MatrixSerializer<double> ser;
                    pt::ptree cameraMatrixNode = calib.get_child("camera");
                    cameraMatrixK = ser.deserialize(cameraMatrixNode);
                    pt::ptree distortionCoefficientsNode = calib.get_child("distortion");
                    distortionCoefficientsD = ser.deserialize(distortionCoefficientsNode);
                    reprojectionError = calib.get<double>("reprojection_error");

                    return;
                }
            };

            /**
             * @brief CameraCalibration constructor with list of preloaded images
             */
            CameraCalibration(CalibrationImageFilenames calibrationImages,
                              int chessboardSquaresWide, // The count of _squares_, not _interior corners_
                              int chessboardSquaresHigh, // The count of _squares_, not _interior corners_
                              int chessboardSquareMm); // The size of each square in mm

            /**
             * @brief getChessboardObjectPoints
             *
             * Given the parameters passed in to the constructor and the assumption that we're looking at the same
             * calibration pattern (which is a chessboard), these points should all be the same for every image.
             *
             * @return a vector of 3d points (which is constructable as an InputArrayOfArrays) to be used with the
             * OpenCV function `calibrateCamera`. These points start with the top-left inner corner and extend in
             * chessboard-space to the right and down
             *
             */
            vector<cv::Point3f> getChessboardObjectPoints() const;


            /**
             * @brief processChessboardImages iterates through each image and attempts to use findChessboardCorners on the image.
             * Note that the CameraCalibration class is not interactive with the user, so ambiguity in the image, which is typically
             * resolved through asking the user if the results apper correct with drawChessboardCorners, which has to happen
             * somewhere else - in a gui app, it could be interactively; in a command-line app, it could be out-of-process with
             * saved images in a directory or some such.
             */
             vector<PointsAndFilename> processChessboardImages();


             /**
              * @brief runCalibration runs the calibration process for a camera given the data from processChessboardImages and getChessboardObjectPoints.
              *
              *
              */

             CalibrationResults::ptr runCalibration();



        private:
            CalibrationImageFilenames m_calibrationImageLocations;
            int m_chessboardSquaresWide;
            int m_chessboardSquaresHigh;
            int m_chessboardSquareMm;
        };

    }
}
#endif // CAMERACALIBRATION_H
