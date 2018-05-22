#include <sstream>
#include "openimage.h"
#include "algorithm/stereo/imagebase.h"
#include "stereoexception.h"

namespace Stereo
{

    const std::string OpenImage::s_key = "open_image";
    const std::string OpenImage::s_version = "1.0";

    OpenImage::OpenImage(const std::string& imageFilename):
        ImagePipelineStepBase()
    {
        memo data;
        data.put("filename", imageFilename);
        data.put("version", OpenImage::s_version);
        m_metadata.add_child(this->key(), data);
    }

    OpenImage::OpenImage(const memo& metadata):
        ImagePipelineStepBase(metadata)
    {}

    OpenImage::OpenImage(const OpenImage &other):
        ImagePipelineStepBase(other.m_metadata)
    {}

    std::string OpenImage::filename() const
    {
        return this->data().get<std::string>("filename");
    }

    void OpenImage::set_filename(const string &imageFilename)
    {
        this->data().put("filename", imageFilename);
    }

    std::string OpenImage::checksum() const
    {
        return this->data().get<std::string>("checksum", "");
    }

    ImagePipelineStepBase::DataList OpenImage::execute(const ImagePipelineStepBase::DataList& inputs)
    {
        //TODO: do we assert that inputs is empty here?
        ImagePipelineStepBase::DataList images;
        // Open the image and check for success
        Algo::ImageBase::ptr image = Algo::ImageBase::load(this->filename());
        std::stringstream sstr;
        sstr << "Could not load image " << this->filename();
        STEREO_LOG_ASSERT(!image->empty(), sstr.str());
        // Set the image in the data
        ImagePipelineStepBase::ImagePipelineData data;
        data.image = image;
        // Metadata: the filename that was given to us
        ImagePipelineStepBase::memo metadata;
        metadata.put("filename", this->filename());
        metadata.put("version", 1);
        // For double-checking sake, calculate a checksum.
        // This can be used to "fingerprint" a file later to be sure it was the same one.
        metadata.put("checksum", data.image->crc());
        // Metadata goes under "open_image" key
        data.metadata.add_child(this->key(), metadata);
        // There's one and only one data in the list.
        images.push_back(data);
        return images;
    }

    static ImagePipelineStepBase::shared_ptr load(ImagePipelineStepBase::memo metadata)
    {
        return OpenImage::shared_ptr(new OpenImage(metadata));
    }
}
