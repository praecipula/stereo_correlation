#ifndef IMAGEPIPELINESTEPBASE_H
#define IMAGEPIPELINESTEPBASE_H

#include "common.h"
#include <memory>
#include <vector>
#include "algorithm/stereo/imagebase.h"
#include <boost/property_tree/ptree.hpp>

namespace Stereo
{
    class ImagePipelineStepBase
    {
    public:

        // The pipeline is assembled as a std List or Vector (anything forward iterable) of these pointers.
        typedef std::shared_ptr<ImagePipelineStepBase> shared_ptr;
        typedef std::weak_ptr<ImagePipelineStepBase> weak_ptr;

        // The type of a pointer to an image used in each step
        typedef std::shared_ptr<Stereo::Algo::ImageBase> image_ptr;
        // A memo of boost property tree
        typedef boost::property_tree::ptree memo;
        // These are the input and output types that get processed.
        // The general flow is for each ImagePipelineStepBase class to modify or
        // transform image in some way and append its metadata to the memo at
        // that step.
        // TODO: perhaps have memo be a property of the pipeline itself? This would be
        // space-efficient, but we lose the command-pattern-like ability to see the
        // modifications at each step.
        struct ImagePipelineData {
            ImagePipelineStepBase::image_ptr image;
            memo metadata;
        };
        // What actually gets passed around is a list of the data struct.
        typedef std::list<ImagePipelineData> DataList;


        /*
     * All image pipeline steps must be default-constructable AND copy-constructable.
     * This class is always default-constructable as an abstract class.
     * Any further information needed to execute must be supplied
     * after construction and before execution
     */
        ImagePipelineStepBase();
        ImagePipelineStepBase(const memo& metadata);
        virtual ~ImagePipelineStepBase() {}

        /* An execution step takes one or more image + "memo" and returns one or more transformed images.
     * Inheritors of this class have the option to either create new images or to transform an existing
     * image and return those; it should not be assumed that the outputs are the same images
     * as the inputs for this reason.
     */
        virtual DataList execute(const DataList& inputs) = 0;

        /**
         * @brief The subclass-specific key defined for the operation
         * @return The key. This is a static member, but each subclass is expected to
         * implement this as a basic return of this static member. [This is highly likely
         * to be inlined by the compiler.]
         */
        virtual std::string key() const = 0;

        /* append this node in a parent property tree. The standard implementation in this parent class makes
         * the following assumptions:
         *
         * 1) The child class has implemented functions get_key and get_version
         * 2) The child class can be iterated over at a single level to be appended.
         */
        virtual void append_for_save(memo& parent);

        /* Construct this node from a property tree memo
         */
        static ImagePipelineStepBase::shared_ptr load(memo metadata);

    protected:
        memo m_metadata;
    };

}
#endif // IMAGEPIPELINESTEPBASE_H
