#ifndef PROJECTPOINTS
#define PROJECTPOINTS

#include <opencv/cv.hpp>
#include <exception>
#include "disparity.h"
#include "model/pointcloud.h"
#include "model/stereo_hardware/camerarelationship.h"

namespace Stereo
{
    namespace Algo
    {
        using namespace cv;

        /**
         * Given a disparity map, project points into a 3d coordinate space.
         *
         */
        class ProjectPoints
        {
        public:
            /**
             * Default constructor.
             * setCameraRelationship must be called before using.
             */
            ProjectPoints();

            /** Construct the points with a given camera hardware configuration
             * @param rel a stereo camera rig setup
             */
            explicit ProjectPoints(Hardware::CameraRelationship::const_ptr rel);

            /**
             * Test if we have a camera relationship
             */

            bool hasCameraRelationship() const{return !!m_rel;}

            /**
             * Function call to perform the projection with the given disparity image
             * @param disp the disparity map
             * @return a 3d-point matrix
             */
            PointCloud operator()(Disparity::const_ptr disp) const;


            /**
             * Set a new camera relationship.
             */
            void setCameraRelationship(Hardware::CameraRelationship::const_ptr rel){m_rel = rel;}


            float removeMe(const Mat& mat, int i, int j) const;

        private:
            Hardware::CameraRelationship::const_ptr m_rel;

        };
    }
}


#endif // PROJECTPOINTS

