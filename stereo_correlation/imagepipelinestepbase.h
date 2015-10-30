#ifndef IMAGEPIPELINESTEPBASE_H
#define IMAGEPIPELINESTEPBASE_H

#include "common.h"
#include "boost/shared_ptr.hpp"
#include <vector>

//forward-declare
namespace Magick {
  class Image;
};

class ImagePipelineStepBase
{
public:

    // The pipeline is assembled as a std List or Vector (anything forward iterable) of these pointers.
    typedef boost::shared_ptr<ImagePipelineStepBase> ptr;

    typedef boost::shared_ptr<Magick::Image> image_ptr;
    typedef std::vector<image_ptr> image_list;

    ImagePipelineStepBase();

    /* An execution step takes one or more ImageMagick images and returns one or more transformed images.
     * Inheritors of this class have the option to either create new images or to transform an existing
     * image and return those; it should not be assumed that the outputs are the same images
     * as the inputs for this reason.
     */
    virtual image_list execute(const image_list& inputs) = 0;

};

#endif // IMAGEPIPELINESTEPBASE_H