#ifndef STEREOMESH_H
#define STEREOMESH_H

#include "openvdb/openvdb.h"
#include <memory>
#include <limits>


namespace Stereo
{
    using namespace std;
    /**
     * PointCloud is our handler for an OpenVDB grid.
     *
     * In our case, the grid is specialized from OpenVDB to store a struct
     * that we use to put arbitrary information in the mesh; rather than use, for instance, a FloatGrid, we
     * use a ControlPointGrid, where a ControlPoint is an application-defined struct. This allows us to track
     * information across frames in video, for example, where a POD float would not.
     *
     * PointCloud has fast copyable semantics due to a pimpl implementation of its contents.
     *
     */
    class PointCloud
    {
    public:
        PointCloud();
        PointCloud(const PointCloud& other);

        // construct with a

        /*
         * This is what is stored in OpenVDB. It acts like a Float; the semantic sense of this is that
         * it represents the isosurface value that represents distance to the level set. It also contains
         * (optional) data about the time history of that value, or whatever else is desired.
         *
         * In vanilla OpenVDB, this is typically managed by using multiple grids: one for each float value
         * that is tracked. A more elegant solution is to track all the information we want in a single grid.
         */
        struct ControlPoint
        {
        public:
            /**
             * No-argument constructor.
             * Creates something at distance of the backgroundControlPoint distance.
             */
            ControlPoint() :
                m_isosurfaceDistanceMM(numeric_limits<float>::max()) {}

            ControlPoint(float isosurfaceDistance) :
                m_isosurfaceDistanceMM(isosurfaceDistance) {}

            ControlPoint(const ControlPoint& other) :
                m_isosurfaceDistanceMM(other.m_isosurfaceDistanceMM) {}

            operator float(){return m_isosurfaceDistanceMM;}

            // The value for a "background" element of the grid
            static ControlPoint backgroundControlPoint();
        private:
            float m_isosurfaceDistanceMM;
        };

        // Our typedefs to work with OpenVDB in a familiar way
        typedef openvdb::tree::Tree4<float, 5, 4, 3>::Type ControlPointTree;
        typedef openvdb::Grid<ControlPointTree> ControlPointGrid;

        /* OpenVDB requires runtime initialization; one of the things that has to happen is to
         * register grid-value types with its system. We have to do that for our custom type as
         * well in order to get everything working; we'll do both here.
         */
        static void initialize();

        shared_ptr<ControlPointGrid> m_grid;

    };
}
#endif // STEREOMESH_H
