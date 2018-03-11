#ifndef IMAGEPIPELINESTEPBASE_H
#define IMAGEPIPELINESTEPBASE_H

#include "common.h"
#include <memory>
#include <vector>
#include "algorithm/stereo/imagebase.h"

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
        // A list of images
        typedef std::vector<image_ptr> image_list;


        /*
     * All image pipeline steps must be default-constructable AND copy-constructable.
     * This class is always default-constructable as an abstract class.
     * Any further information needed to execute must be supplied
     * after construction and before execution
     */
        ImagePipelineStepBase();
        virtual ~ImagePipelineStepBase() {}

        /* An execution step takes one or more images and a "memo" and returns one or more transformed images.
     * Inheritors of this class have the option to either create new images or to transform an existing
     * image and return those; it should not be assumed that the outputs are the same images
     * as the inputs for this reason.
     */
        virtual image_list execute(const image_list& inputs) = 0;

    };

}
#endif // IMAGEPIPELINESTEPBASE_H
