#include "saveimage.h"
#include "strcon.h"
#include "stereoexception.h"

namespace Stereo
{

    const std::string SaveImage::s_key = "save_image";
    const std::string SaveImage::s_version = "1.0";

    SaveImage::SaveImage():
        ImagePipelineStepBase(this->key(), this->version()), PipelineStepAcceptingImage()
    {}

    SaveImage::SaveImage(const std::string& imageFilename):
        ImagePipelineStepBase(this->key(), this->version()), PipelineStepAcceptingImage()
    {
        this->mutable_data().put("filename", imageFilename);
    }

    SaveImage::SaveImage(const memo& metadata):
        ImagePipelineStepBase(metadata), PipelineStepAcceptingImage()
    {}

    SaveImage::SaveImage(const SaveImage &other):
        ImagePipelineStepBase(other.m_metadata), PipelineStepAcceptingImage(other.get_image())
    {}

    std::string SaveImage::filename() const
    {
        return this->data().get<std::string>("filename", "");
    }

    std::string SaveImage::describe() const
    {
        std::stringstream stream;
        stream << "Save: " << this->filename();
        return stream.str();
    }

    /*
 * Save the image at m_filename, and then return an empty image_list.
 * If m_filename is unset, this will prompt the user for a filename.
 */
    void SaveImage::execute(const ImagePipeline& pipeline)
    {
        STEREO_LOG_ASSERT(!this->filename().empty(), "No filename for image saving");
        STEREO_LOG_ASSERT(this->get_image(), "No image provided for saving");
        this->get_image()->save(this->filename());
        //inputs.front().image->save(m_filename);
        // This terminates the execution - there are no more images to pass.
        // Just pass back an empty data list.
        //return ImagePipelineStepBase::DataList();
    }
}
