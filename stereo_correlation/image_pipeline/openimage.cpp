#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "openimage.h"
#include "algorithm/stereo/imagebase.h"
#include "stereoexception.h"
#include "image_pipeline/imagepipeline.h"


namespace Stereo
{

    const std::string OpenImage::s_key = "open_image";
    const std::string OpenImage::s_version = "1.0";

    OpenImage::OpenImage(const std::string& imageFilename):
        ImagePipelineStepWithCommonImpl(this->key(), this->version())
    {
        this->mutable_data().put("filename", imageFilename);
    }

    OpenImage::OpenImage(const memo& metadata):
        ImagePipelineStepWithCommonImpl(metadata)
    {}

    OpenImage::OpenImage(const OpenImage &other):
        ImagePipelineStepWithCommonImpl(other.m_metadata)
    {}

    std::string OpenImage::filename() const
    {
        return this->data().get<std::string>("filename", "");
    }

    void OpenImage::set_filename(const string &imageFilename)
    {
        this->mutable_data().put("filename", imageFilename);
    }

    OpenImage::hex_string OpenImage::checksum() const
    {
        return this->data().get<OpenImage::hex_string>("checksum", "");
    }

    void OpenImage::set_checksum(const Algo::ImageBase::checksum &sum)
    {
        /* We can't use data accessor, as it's public interface is a const
         * copy of what's at our key, so have to do this manually.
         * Additionally, we want to convert from the image base's checksum
         * (uint32_t at this time) to a hex string so it looks more
         * checksummy in the JSON.
         */
        std::stringstream stream;
        stream <<"0x"
               << std::setfill ('0') << std::setw(sizeof(Algo::ImageBase::checksum)*2)
               << std::hex << sum;
        std::string converted(stream.str());
        this->mutable_data().put<OpenImage::hex_string>("checksum", converted);
    }

    std::string OpenImage::describe() const
    {
        std::stringstream stream;
        stream << "Open: " << this->filename();
        return stream.str();
    }

    void OpenImage::execute(const ImagePipeline& pipeline)
    {
        // TODO: do we assert that inputs is empty here?
        // This is basically the first reasonable thing we do at this point.
        ImagePipeline::OrderedSteps deps = pipeline.get_dependencies(this);
        // Open the image and check for success
        Algo::ImageBase::ptr image = Algo::ImageBase::load(this->filename());
        std::stringstream sstr;
        sstr << "Could not load image " << this->filename();
        STEREO_LOG_ASSERT(!image->empty(), sstr.str());
        // Set our checksum for sanity for loading in the future.
        this->set_checksum(image->crc());
    }


}
