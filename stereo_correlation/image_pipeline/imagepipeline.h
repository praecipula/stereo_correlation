#ifndef IMAGEPIPELINE_H
#define IMAGEPIPELINE_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace Stereo
{

    /**
 * @brief The ImagePipeline class is the central location to manage incoming signals and calculate what
 * needs to be done to an image in order to properly process it. It constructs a set of operations, which
 * act through the ImagePipeline as mediator in order to determine what exactly should be done to a pair
 * of images in order to create the proper output.
 *
 * @ The intent of this is to use derivatives of ImagePipelineStepBase to characterize the operations to be
 * done on an image; these will describe precisely what should be done when in order to provide the correct,
 * desired output. The ImagePipeline "listens" to the rest of the system in order to determine that certain
 * events (e.g. a dual-correlation being reached) should be used to create or modify a pipeline operation.
 *
 * Pipeline steps are themselves agnostic of the type of 3d image that is being created. This allows for
 * multiple pipelines to exist, or, more specifically, for ImagePipeline to exist as a tree. The tree will
 * then be processed which allows, for instance, one common set of operations which fork to create an
 * anaglyph and a JPS file from the same pipeline.
 */
    class ImagePipeline
    {
        typedef boost::adjacency_list<boost::vecS, boost::setS, boost::directedS> graph;

    public:
      ImagePipeline();

      graph m_graph;
    };

}
#endif // IMAGEPIPELINE_H
