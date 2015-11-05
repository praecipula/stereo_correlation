#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <string>
#include "coordinate.h"
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_multiset_of.hpp>
#include <boost/bimap/multiset_of.hpp>

namespace Stereo
{
    /*
     * For our tagged coordinates, we need to have a comparison for ordering's sake.
     * It doesn't really make sense, in a general way, to define a < operator for
     * Coordinate - what is a coordinate that is "less than" another one look like?
     * Therefore, we just need some sort of strict weak ordering.
     *
     * This class arbitrarily uses a "scanline" interpretation - vertical comparison,
     * then horizontal comparison, of the coordinates to define an ordering.
     */
    struct CoordinateComparison
    {
        bool operator()(const Coordinate& first, const Coordinate& second) const
        {
            if (first.y() < second.y()) return true;
            if (first.x() < second.x()) return true;
            return false;
        }
    };

    /*
     * The Workspace class is the model of our MVC app; it stores the model state, such as
     * the current left and right images, and the image pipeline constructed for this
     * image.
     *
     * It has all the functionality for saving the model and loading from a file as well.
     * This means that I'm not choosing to make it a Qt object; it could be used from
     * a command line interface, for instance. Instead it is related to QWorkspaceMediator,
     * which delegates to a Workspace for all storage, but communicates to the rest
     * of the app with signals and slots and all that Qt goodness.
     */
    class Workspace
    {
    public:

        /*
         * A bidirectional map, where one or more tags can be associated with one or more coordinates.
         * Tags are looked up by hash O(1) and coordinates by binary search O(log n), using an
         * arbitrary ordering scheme for this purpose.
         */
        typedef boost::bimap<
        boost::bimaps::unordered_multiset_of<std::string>,
        boost::bimaps::multiset_of<Coordinate, CoordinateComparison> > tagged_coordinates;

      Workspace();

      std::string serialize();

    private:
      /*
       * The left and right image coordinates are tagged in order to be associated with
       * each other. For instance, there is a correlation that is important to align the
       * images on the image plane. This coordinate is tagged, because it is relevant to
       * the "align" processing step, not necessarily all. This is an attempt to add
       * metadata to coordinates without, for instance, exploding a subclass heirarchy
       * or modifying the definition of a "coordinate", which may or may not even be used
       * to refer to a correlation point.
       */
      tagged_coordinates left_image_coordinates;
      tagged_coordinates right_image_coordinates;

    };

}

#endif // WORKSPACE_H
