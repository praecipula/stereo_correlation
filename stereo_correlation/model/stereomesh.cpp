#include "stereomesh.h"

namespace Stereo
{
    void StereoMesh::initialize()
    {
        // Single-initialization guard
        bool isInitialized = false;
        if (!isInitialized)
        {
            // Initialize OpenVDB
            openvdb::initialize();
            // Register the grid in OpenVDB
            ControlPointGrid::registerGrid();
            isInitialized = true;
        }
    }

    StereoMesh::StereoMesh()
    {

    }

}
