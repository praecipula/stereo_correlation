#include "pointcloud.h"
namespace Stereo
{
    void PointCloud::initialize()
    {
        // Single-initialization guard
        static bool isInitialized = false;
        if (!isInitialized)
        {
            // Initialize OpenVDB
            openvdb::initialize();
            // Register the grid in OpenVDB
            //ControlPointGrid::registerGrid();
            isInitialized = true;
        }
    }


    PointCloud::PointCloud():
        m_grid(new ControlPointGrid(PointCloud::ControlPoint::backgroundControlPoint()))
    {
        m_grid->setGridClass(openvdb::GRID_LEVEL_SET);
    }


    PointCloud::ControlPoint PointCloud::ControlPoint::backgroundControlPoint()
    {
        static ControlPoint backgroundControlPoint;
        return backgroundControlPoint;
    }
}
