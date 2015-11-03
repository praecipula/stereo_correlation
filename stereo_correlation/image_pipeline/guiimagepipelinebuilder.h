#ifndef GUIIMAGEPIPELINEBUILDER_H
#define GUIIMAGEPIPELINEBUILDER_H

#include "imagepipelinestepbase.h"
#include <boost/function.hpp>

namespace Stereo
{
    /*
     * The GuiImagePipelineBuilder has 2 main roles:
     *
     * Construction of an image pipeline step. In other words, a class that encapsulates
     * a single source of object construction.
     *
     * Resolution of construction dependencies. This means that the builder bridges
     * the non-qt pipeline (which may be later used to, for example, design a
     * command-line scripting engine) and the UI in order to resolve prerequisites -
     * for instance, OpenImage needs to be constructed with a filename.
     *
     * This may turn into an abstract factory pattern, if we do make a scriptable
     * interface.
     */
class GuiImagePipelineBuilder
{
public:
    GuiImagePipelineBuilder();

    typedef boost::function<ImagePipelineStepBase::ptr(GuiImagePipelineBuilder*)> fn_ptr;

    ImagePipelineStepBase::ptr buildOpenImage();
    ImagePipelineStepBase::ptr buildSaveImage();

};
}
#endif // GUIIMAGEPIPELINEBUILDER_H
