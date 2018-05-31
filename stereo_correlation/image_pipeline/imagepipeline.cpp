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
        m_graph(),
        m_reverse_map()
    {

    }

    ImagePipeline::PipelineStepId ImagePipeline::add_node(ImagePipelineStepBase::shared_ptr node)
    {
        /* Add to the graph. This takes ownership (and everyone with an external shared_ptr should
            know that dereferencing is safe and should use the returned ID.
        */
        PipelineStepId id = add_vertex(this->m_graph);
        // Insert the value of the shared ptr's address as the key to get the id later if needed
        // TODO: I now think I prefer putting the id as a void pointer in the ImagePipelineStepBase.
        // Then we can static_cast it here to the right opaque type. We should document well (or maybe
        // allow access only via friend class) that this is what it's for and here there be monsters.
        this->m_reverse_map.insert(std::make_pair(node.get(), id));
        // Set the processing step in the node properties
        this->m_graph[id].processing_step = node;
        return id;
    }

    ImagePipeline::PipelineStepId ImagePipeline::id_for_pipeline_step_ptr(const ImagePipelineStepBase* for_this_node) const
    {
        try {
            return m_reverse_map.at(for_this_node);
        } catch (const std::range_error& e) {
            std::stringstream sstr;
            sstr << "Pointer to processing step missing from pipeline reverse lookup map " << for_this_node;
            STEREO_LOG_ASSERT(false, sstr.str());
        }
    }

    void ImagePipeline::add_dependency(PipelineStepId dependant, PipelineStepId prerequisite)
    {
        std::pair<ImagePipeline::PipelineEdgeId, bool> step_added_data;
        // Draw the edge backwards pointing from dependant to prerequisite.
        step_added_data = add_edge(dependant, prerequisite, this->m_graph);
        // Likely in the future add something like "last processed time" to show the edge has not been
        // processed.
    }

    ImagePipeline::OrderedSteps ImagePipeline::get_dependencies(const ImagePipelineStepBase* const for_this_node) const
    {
        ImagePipeline::OrderedSteps dependencies;
        ImagePipeline::PipelineStepId id = id_for_pipeline_step_ptr(for_this_node);
        dependency_iter_range range = out_edges(id, this->m_graph);
        for(ImagePipeline::dependencies_iter i = range.first;
            i != range.second;
            ++i)
        {
            PipelineStepId dependencies_id = target(*i, this->m_graph);
            ImagePipelineStepBase::weak_ptr weak(this->m_graph[dependencies_id].processing_step);
            dependencies.push_back(weak);
        }
        return dependencies;
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
            processing_steps.push_back((it->lock())->describe());
        }
        return processing_steps;
    }

}
