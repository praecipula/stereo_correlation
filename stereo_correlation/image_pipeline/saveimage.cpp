#include "saveimage.h"
#include "strcon.h"
#include "stereoexception.h"

namespace Stereo
{
    SaveImage::SaveImage(const std::string filename):
        ImagePipelineStepBase(), m_filename(filename)
    {}

    /*
 * Save the image at m_filename, and then return an empty image_list.
 * If m_filename is unset, this will prompt the user for a filename.
 */
    ImagePipelineStepBase::image_list SaveImage::execute(const ImagePipelineStepBase::image_list &inputs)
    {
        STEREO_LOG_ASSERT(!m_filename.empty(), "No filename for image saving");
        inputs.front()->save(m_filename);
        return image_list();
    }
}
