#ifndef OPENIMAGE_H
#define OPENIMAGE_H

#include "imagepipelinestepbase.h"

namespace Stereo
{
    /*
     * More or less the first step in any image pipeline.
     *
     * Open the image and deliver it to ImageMagick so that it can operate on the image.
     */
    class OpenImage : public ImagePipelineStepBase
    {
    public:
        typedef std::shared_ptr<OpenImage> shared_ptr;

        static const std::string s_key;
        static const std::string s_version;

        OpenImage(const std::string& imageFilename);
        OpenImage(const memo& metadata);
        OpenImage(const OpenImage& other);
        virtual ~OpenImage() {}

        std::string filename() const;
        void set_filename(const std::string& imageFilename);

        // Typedef for clarity
        typedef std::string hex_string;
        hex_string checksum() const;

        // Return a const copy of the metadata under the key for quick property inspection.
        ImagePipelineStepBase::memo data() const {return this->m_metadata.get_child(this->key());}

        virtual std::string describe() const;
        virtual void execute(const ImagePipeline& pipeline);
        virtual std::string key() const {return OpenImage::s_key;}
        static ImagePipelineStepBase::shared_ptr load(memo metadata);

    protected:
        void set_checksum(const Algo::ImageBase::checksum& sum);
        // Return a mutable reference to the data under the key for quick property inspection.
        // This helps a lot in cases where we accidentally would put() to the data returned from the
        // const version, which is a copy for the outside world to prevent dangling references.
        ImagePipelineStepBase::memo& mutable_data() {return this->m_metadata.get_child(this->key());}
    };

}
#endif // OPENIMAGE_H
