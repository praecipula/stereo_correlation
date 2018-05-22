#include <boost/graph/topological_sort.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "imagepipeline.h"
#include "stereoexception.h"

namespace Stereo
{

    using namespace boost;
    namespace pt = boost::property_tree;

    ImagePipeline::ImagePipeline():
        m_graph()
    {

    }

    ImagePipeline::PipelineStepId ImagePipeline::add_node(ImagePipelineStepBase::shared_ptr node)
    {
        PipelineStepId id = add_vertex(this->m_graph);
        this->m_graph[id].processing_step = node;
        return id;
    }

    void ImagePipeline::add_dependency(PipelineStepId dependant, PipelineStepId prerequisite)
    {
        std::pair<ImagePipeline::PipelineEdgeId, bool> step_added_data;
        // Draw the edge backwards pointing from dependant to prerequisite.
        step_added_data = add_edge(dependant, prerequisite, this->m_graph);
        // Likely in the future add something like "last processed time" to show the edge has not been
        // processed.
    }

    /* Topo sort to get the dependency ordering.
     * For each node in the sorted list
     * Get incident edges, if any
     * Get their execution results, if any
     * Execute the node and cache/store its results in the node
     */

    ImagePipeline::OrderedSteps ImagePipeline::evaluate_processing_order() const
    {
        typedef std::list<ImagePipeline::PipelineStepId> StepIds;
        StepIds steps;
        topological_sort(this->m_graph, std::back_inserter(steps));
        ImagePipeline::OrderedSteps unowned_steps;
        // Since we drew the dependency graph "backwards" with edges to point at the
        // prerequisites from the depeandant, the reverse topological order is
        // already in the evaluation order (double-negative the fact that the topo sort
        // returns in reverse topological order)
        for (StepIds::iterator i=steps.begin();
             i!=steps.end();
             ++i)
        {
            ImagePipelineStepBase::weak_ptr weak(this->m_graph[*i].processing_step);
            unowned_steps.push_back(weak);
        }
        return unowned_steps;
    }

    pt::ptree ImagePipeline::prepare_to_serialize() const
    {
        // Metadata: the version of the ImagePipeline, date saved, etc...
        pt::ptree tree;
        tree.put("image_pipeline_version", "1.0");
        // Topologically sorted pipeline
        ImagePipeline::OrderedSteps steps = this->evaluate_processing_order();
        for (ImagePipeline::OrderedSteps::iterator i = steps.begin();
             i != steps.end();
             ++i)
        {
            ImagePipelineStepBase::weak_ptr p = *i;
            p.lock()->append_for_save(tree);
        }
        return tree;
    }

    std::string ImagePipeline::serialize() const
    {
        std::string serialized;
        pt::ptree tree = this->prepare_to_serialize();
        pt::write_json(serialized, tree);
        return serialized;
    }

    void ImagePipeline::save(std::string filename)
    {
        pt::ptree tree = this->prepare_to_serialize();
        pt::write_json(filename, tree);
    }

    ImagePipeline load(std::string filename)
    {
        //TODO
    }

    std::list<std::string> ImagePipeline::dry_run() const
    {
        std::list<std::string> processing_steps;
        OrderedSteps steps = this->evaluate_processing_order();
        for (OrderedSteps::iterator it = steps.begin();
             it != steps.end();
             ++it)
        {
            processing_steps.push_back((it->lock())->key());
        }
        return processing_steps;
    }

}
