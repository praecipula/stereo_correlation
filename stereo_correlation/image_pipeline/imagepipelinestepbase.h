#ifndef IMAGEPIPELINESTEPBASE_H
#define IMAGEPIPELINESTEPBASE_H

#include "common.h"
#include <memory>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include "algorithm/stereo/imagebase.h"

namespace Stereo
{
    // Forward-declare this class
    class ImagePipeline;

    class ImagePipelineStepBase
    {
    public:

        // The pipeline is assembled as a std List or Vector (anything forward iterable) of these pointers.
        // TODO: consider changing this to ptr instead of shared_ptr to fit conventions
        typedef std::shared_ptr<ImagePipelineStepBase> shared_ptr;
        typedef std::weak_ptr<ImagePipelineStepBase> weak_ptr;

        // A memo of boost property tree
        typedef boost::property_tree::ptree memo;

        /*
        * Any further information needed to execute must be supplied
        * after construction and before execution
        */
        ImagePipelineStepBase(std::string subclass_key, std::string subclass_version);
        ImagePipelineStepBase(const memo& metadata);
        virtual ~ImagePipelineStepBase() {}

        /**
         * Construct this node from a property tree memo. This is used when opening a
         * serialized version of the image pipeline.
         */
        static ImagePipelineStepBase::shared_ptr load(memo metadata);

        /****
         * Serializing the step
         ****/

        /* append this node in a parent property tree. The standard implementation in this parent class here makes
         * the following assumptions:
         *
         * 1) The child class has implemented functions get_key and get_version
         * 2) The child class can be iterated over at a single level to be appended.
         *
         * Subclasses can override this, but in general should not have to if there is any
         * sane modification of the parent implementation that does not break backwards
         * compatibility at the serialized file.
         */
        virtual void append_for_save(memo& parent);

        /**
         * @brief Describe the operation.
         *
         * This is a very brief string that describes the step used to (for instance) execute a dry
         * run and print the steps to console.
         * @return
         */
        virtual std::string describe() const = 0;

        /**
        * @brief execute this node in the graph.
        *
        * The inputs and outputs expected by this node are either set/constructed on the node (think
        * supplying a filename to OpenImage) or can be determined from the graph (think correlating
        * the left and right images to produce a single image). In general, the DAG is a directed
        * dependency graph, so there is no result other than to change the current state of the
        * DAG during processing. So, for instance, after correlation, we might end up with one node
        * for the correlation with the metadata we want, and that node knows how to get the correct
        * left and right images (even if it's going to an upstream node to get them) and this node
        * can be upstream of both a create anaglyph and create JPS downstream node, both of which
        * expect to get the images.
        * @return none
        */
        virtual void execute(const ImagePipeline& pipeline) = 0;

        /**
        * @brief The subclass-specific key defined for the operation
        * @return The key. This is a static member, but each subclass is expected to
        * implement this as a basic return of this static member. [This is highly likely
        * to be inlined by the compiler.]
        *
        * The point of making it pure virtual is to remind subclasses to do this.
        */
        virtual std::string key() const = 0;

        /**
         * @brief The subclass-specific version for the operstion
         * @return The version. This is a static member, but each class is expected to
         * implement this as a basic return of this static member
         */
        virtual std::string version() const = 0;


    protected:
        memo m_metadata;
    };

}
#endif // IMAGEPIPELINESTEPBASE_H
