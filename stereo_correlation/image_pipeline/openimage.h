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
    class OpenImage : public ImagePipelineStepWithCommonImpl<OpenImage>
    {
    public:
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

        virtual std::string describe() const;
        virtual void execute(const ImagePipeline& pipeline);

    protected:
        void set_checksum(const Algo::ImageBase::checksum& sum);
    };

}
#endif // OPENIMAGE_H
