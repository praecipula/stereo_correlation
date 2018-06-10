#ifndef IMAGEPIPELINE_H
#define IMAGEPIPELINE_H

#include <unordered_map>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/property_tree/ptree.hpp>
#include "imagepipelinestepbase.h"

namespace Stereo
{

    using namespace boost;
    namespace pt = boost::property_tree;

    /**
 * @brief The ImagePipeline class is the central location to manage incoming signals and calculate what
 * needs to be done to an image in order to properly process it. It constructs a set of operations, which
 * act through the ImagePipeline as mediator in order to determine what exactly should be done to a pair
 * of images from the camera in order to create the proper output.
 *
 * The intent of this is to use subclasses of ImagePipelineStepBase to characterize the operations to be
 * done on an image; these will describe precisely what should be done when in order to provide the correct
 * desired output.
 *
 * Pipeline steps are themselves agnostic of the type of 3d image that is being created. This allows for
 * multiple pipelines to exist, or, more specifically, for ImagePipeline to exist as any DAG. This will
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
         * Reserved for future use
         */
        struct PipelineEdgeProperties {
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

      // How many pipeline steps are there total (how many nodes)
      int size() const {return num_vertices(this->m_graph);}

      /** Add a node to the graph. It is initially not dependent on other nodes, nor do any nodes
      * depend on it.
      * ***NOTE*** Transfers ownership from wherever the node was constructed to the pipeline.
      * All other operations will provide and return weak pointers.
      * Returns an ID which can or should be used for all operations outside of the pipeline.
      *
      * (All graph operations must occur on nodes that have already been added to the graph)
      */
      PipelineStepId add_node(ImagePipelineStepBase::shared_ptr node);

      /**
       * @brief Given a pipeline step pointer, find the pipeline ID for it.
       * @param for_this_node the node to get the ID for
       * @return the step id.
       *
       * Sadly, once a pointer goes into the graph, it gets an ID type that is upstream-dependent
       * on the type itself. This means an object of that type can't use its own ID (since there's
       * no good way to define the processing step as having access to that type)
       *
       * It's possible to get this at runtime, though, so that enables the pipeline steps to take advantage
       * of the upstream graph if they need to find out properties (and enables us to use this convenient
       * method for a better interface).
       *
       * We also consider the graph to own its nodes and only return weak pointers to them, so since this
       * structure already controls the scope of this data, we can be sure that the smart pointer won't
       * deallocate until the whole graph deallocates, at least in theory.
       */
      PipelineStepId id_for_pipeline_step_ptr(const ImagePipelineStepBase* const for_this_node) const;

      /**
       * Construct the processing step dependency, such that the output of prerequisite will be fed into the
       * input of dependant when the processing DAG is evaluated.
       */
      void add_dependency(PipelineStepId dependant, PipelineStepId prerequisite);

      /**
       * @brief Get dependencies for a given node.
       * @param for_this_node the node to get the dependencies for
       * @return a list of weak pointers to the dependencies.
       */
      OrderedSteps get_dependencies(const ImagePipelineStepBase* for_this_node) const;

      /**
       * Build the topologically-sorted dependency list.
       */
      OrderedSteps evaluate_processing_order() const;

      /**
       * Do a dry run:
       * This will iterate over the nodes in the same order that we would execute them,
       * but just concatenate the execution order of the nodes in the graph.
       */
      std::list<std::string> dry_run() const;

      //*** Loading and saving the model state
    protected:
      pt::ptree prepare_to_serialize() const;
    public:
      std::string serialize() const;
      void save(std::string filename);
      static ImagePipeline load(std::string filename);

      DependencyGraph m_graph;
      // This map allows us to look at a step from the "outside" not as an ID and get its
      // associated graph id for graph ops.

      // When looking for dependencies, this is the type of iterator returned
      typedef boost::graph_traits<DependencyGraph>::out_edge_iterator dependencies_iter;
      // When looking for a set of dependencies, this is the range type returned.
      typedef std::pair<dependencies_iter, dependencies_iter> dependency_iter_range;

      typedef std::unordered_map<const ImagePipelineStepBase*, PipelineStepId> reverse_map;
      reverse_map m_reverse_map;

    };

}
#endif // IMAGEPIPELINE_H
