#ifndef IMAGEPIPELINE_H
#define IMAGEPIPELINE_H

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
 * This combination of pipeline operations, which are post-processor agnostic, can then be invoked by a post-
 * processor to compile a result. For instance, an Anaglyph post-processor can use the information to determine
 * what operations are interesting or intended for it to execute in order to produce an anaglyph which best
 * represents the image. Note that not all operations have meaning to all post-processors, nor do
 */
    class ImagePipeline
    {
    public:
      ImagePipeline();
    };

}
#endif // IMAGEPIPELINE_H
