#include "projectpoints.h"

#include <QString>
#include <QFile>
#include <QTextStream>

namespace Stereo
{
    namespace Algo
    {
        using namespace cv;
        ProjectPoints::ProjectPoints():
            m_rel()
        {}

        ProjectPoints::ProjectPoints(Hardware::CameraRelationship::const_ptr rel):
            m_rel(rel)
        {}

        float ProjectPoints::removeMe(const Mat& mat, int i, int j) const
        {
            return mat.at<float>(i, j);
        }

        PointCloud ProjectPoints::operator()(Disparity::const_ptr disp) const
        {
            if(! hasCameraRelationship())
            {
                throw runtime_error("ProjectPoints needs a camera relationship before being called");
            }
            PointCloud cloud;
            /* Make missing points be the average of their boundary using a flood-fill operation to find
             * contiguous points in the missing field. Border points are ignored.
             */


            QString debugFilename = "/tmp/points.txt";
            QFile testFile(debugFilename);
            testFile.open(QIODevice::ReadWrite);
            QTextStream stream(&testFile);

            // Reproject to 3d
            Mat pointData(disp->rows, disp->cols, CV_32FC3, 0.0);
            /* Important: there seems to be a bug in reprojectImageTo3D. See http://stackoverflow.com/questions/22418846/reprojectimageto3d-in-opencv */
            // reprojectImageTo3D(*disp, pointData, m_rel->Q(), false, CV_32F);

            /* What do you do? Roll your own (or roll one someone else contributed, and work with it). */
            cv::Mat_<double> vec_tmp(4,1);
            for(int y=0; y<disp->rows; ++y) {
                for(int x=0; x<disp->cols; ++x) {
                    vec_tmp(0)=x; vec_tmp(1)=y; vec_tmp(2)=disp->at<float>(y,x); vec_tmp(3)=1;
                    vec_tmp = m_rel->Q()*vec_tmp;
                    vec_tmp /= vec_tmp(3);
                    cv::Vec3f &point = pointData.at<cv::Vec3f>(y,x);

                    point[0] = vec_tmp(0);
                    point[1] = vec_tmp(1);
                    point[2] = vec_tmp(2);

                    stream << point[0] << "," << point[1] << "," << point[2] << "|";
                }
                stream << endl;
            }
            testFile.close();
            // Insert those points into the point cloud as an isosurface.
            return cloud;




            // TODO: figure out if VolumeToMesh needs a closed (watertight) volume. If not, OK. If so:
            /* Make a (conceptual) bounding box of radius (max_depth or max_width). The projected 3D
             * surface should be contained entirely within this box. Tying into a 3D graphics paradigm,
             * I'll call this the "skybox".
             * Make an assumption: all points interior to known points of the bounding rectangle of the
             * surface projection should be interpolated. That is, we have discrete points, with no knowledge
             * of intermediate points in between what should be a continuous surface. Any unknown cells in OpenVDB
             * will be the planar interpolation of its 3 nearest defined points. (This follows well: the distance function
             * is _always_ the distance to the plane defined by the 3 nearest points).
             * We know what the look vector is for the cameras, as we define it to be along the -z axis.
             * We can also project the vector back to the skybox intersection from the focal point, as
             * at this point we know the points along the border of the disparity map. Therefore, we
             * define the isosurface as:
             * Each point along the skybox where the projection to the skybox is outside the point cloud border
             * Each point along the image, along with interpolated planar data if necessary (I don't know if
             * OpenVDB at this point requires elementwise continuity - in which case we define the points - or if
             * it can interpolate gaps in its data).
             * The points along the plane of depth projection between the borders of the point cloud and the skybox.
             * This formulation creates a closed (watertight) box with an interior, concave protrusion from the point
             * cloud; it is also a formulation that allows for "panoramic" or multi-view, multi-point-cloud combinations
             * of a single scene - simply piecewise project point clouds (with logic for overlaps) to handle the scene.
             */
            /* Construct an openvdb cube */
            /* Insert the point cloud in the cube */
            /* Project the point cloud edge polygon onto the cube */
            /* Cut out the interior points of the polygon */
            /* Assign "close" level set values to outgoing vectors through edge points to cube */
            /* Profit, or interpolate missing values, depending on if this is enough. */
        }


    }
}
