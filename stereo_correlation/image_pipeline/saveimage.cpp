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
    void SaveImage::execute(const ImagePipeline& pipeline)
    {
        //STEREO_LOG_ASSERT(!m_filename.empty(), "No filename for image saving");
        //inputs.front().image->save(m_filename);
        // This terminates the execution - there are no more images to pass.
        // Just pass back an empty data list.
        //return ImagePipelineStepBase::DataList();
    }
}
