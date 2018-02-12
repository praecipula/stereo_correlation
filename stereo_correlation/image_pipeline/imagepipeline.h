#ifndef IMAGEPIPELINE_H
#define IMAGEPIPELINE_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "imagepipelinestepbase.h"

namespace Stereo
{

    /**
 * @brief The ImagePipeline class is the central location to manage incoming signals and calculate what
 * needs to be done to an image in order to properly process it. It constructs a set of operations, which
 * act through the ImagePipeline as mediator in order to determine what exactly should be done to a pair
 * of images in order to create the proper output.
 *
 * @ The intent of this is to use subclasses of ImagePipelineStepBase to characterize the operations to be
 * done on an image; these will describe precisely what should be done when in order to provide the correct
 * desired output.
 *
 * Pipeline steps are themselves agnostic of the type of 3d image that is being created. This allows for
 * multiple pipelines to exist, or, more specifically, for ImagePipeline to exist as any DAG. The tree will
 * then be processed which allows, for instance, one common set of operations which fork to create an
 * anaglyph and a JPS file from the same pipeline, or a series of steps in parallel that merge into a
 * single trunk (for instance, open and undistort images, then combine them.)
 */
    class ImagePipeline
    {
        // The graph itself, which we use to do a topological sort of dependencies in order to execute
        // the correct image processing set.
        typedef boost::adjacency_list<boost::vecS, boost::setS, boost::directedS> dependency_graph;

    public:
      ImagePipeline();

      void execute();
      void dry_run();

      // How many pipeline steps are there total (how many nodes)
      int size() const {return num_vertices(this->m_graph);}

      // Add a dependency - this is a node
      void add_dependency(ImagePipelineStepBase::ptr dependant, ImagePipelineStepBase::ptr prerequisite);

      dependency_graph m_graph;
    protected:
    };

}
#endif // IMAGEPIPELINE_H
