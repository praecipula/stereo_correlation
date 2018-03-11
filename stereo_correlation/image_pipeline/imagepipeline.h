#ifndef IMAGEPIPELINE_H
#define IMAGEPIPELINE_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/property_tree/ptree.hpp>
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

        /*
         * Each vertex has a processing step associated with it.
         * Note that this shared pointer is the logical owner of the processing step;
         * all the return values in the image pipeline should return weak pointers to signify
         * that if the step is deleted / moved out of the graph it can and will lose scope
         * and be freed.
         */
        struct PipelineVertexProperties {
            ImagePipelineStepBase::shared_ptr processing_step;
        };

        /*
         * Each edge has (maybe) an image and (maybe) metadata associated with it.
         * re: maybe an image, think of a "Save this image to a file" step that effectively
         * has no output
         * re: maybe metadata, some steps are fully self-contained and need no data to work
         * effectively.
         */
        struct PipelineEdgeProperties {
            ImagePipelineStepBase::image_ptr image;
            boost::property_tree::ptree metadata;
        };


    public:

      //This is the type of the graph that we store internally for keeping track of the pipeline
      typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
        PipelineVertexProperties, PipelineEdgeProperties> DependencyGraph;
      // This is the type of the ID that comes back from adding a vertex (for bundled properties, say)
      // Since the most significant element in the vertex is an image pipeline step base, call it a step id.
      typedef boost::graph_traits<DependencyGraph>::vertex_descriptor PipelineStepId;
      // Thi sis the type of ID from the edges for those bundled properties
      typedef boost::graph_traits<DependencyGraph>::edge_descriptor PipelineEdgeId;
      // A topological sort returns an in-order-of-steps dependency list
      typedef std::list<ImagePipelineStepBase::weak_ptr> OrderedSteps;


      ImagePipeline();

      //void execute();
      //void dry_run();

      // How many pipeline steps are there total (how many nodes)
      int size() const {return num_vertices(this->m_graph);}

      /* Add a node to the graph. It is initially not dependent on other nodes, nor do any nodes
      * depend on it.
      * ***NOTE*** Transfers ownership from wherever the node was constructed to the pipeline.
      * All other operations will provide and return weak pointers.
      * Returns an ID which can or should be used for all operations outside of the pipeline.
      *
      * (All graph operations must occur on nodes that have already been added to the graph)
      */
      PipelineStepId add_node(ImagePipelineStepBase::shared_ptr node);

      /*
       * Construct the processing step dependency, such that the output of prerequisite will be fed into the
       * input of dependant when the processing DAG is evaluated.
       */
      void add_dependency(PipelineStepId dependant, PipelineStepId prerequisite);

      /*
       * Build the topologically-sorted dependency list.
       */
      OrderedSteps evaluate_processing_order() const;



      DependencyGraph m_graph;
    protected:

    };

}
#endif // IMAGEPIPELINE_H
