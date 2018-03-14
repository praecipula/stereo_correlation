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
        virtual ~ImagePipelineStepBase() {}

        /* An execution step takes one or more image + "memo" and returns one or more transformed images.
     * Inheritors of this class have the option to either create new images or to transform an existing
     * image and return those; it should not be assumed that the outputs are the same images
     * as the inputs for this reason.
     */
        virtual DataList execute(const DataList& inputs) = 0;

    };

}
#endif // IMAGEPIPELINESTEPBASE_H
