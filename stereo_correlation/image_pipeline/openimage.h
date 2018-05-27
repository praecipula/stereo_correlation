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
        static const std::string s_key;
        static const std::string s_version;

        OpenImage(const std::string& imageFilename);
        OpenImage(const memo& metadata);
        OpenImage(const OpenImage& other);
        virtual ~OpenImage() {}

        std::string filename() const;
        void set_filename(const std::string& imageFilename);

        std::string checksum() const;

        virtual std::string describe() const;
        virtual DataList execute(const DataList& inputs);
        virtual std::string key() const {return OpenImage::s_key;}
        static ImagePipelineStepBase::shared_ptr load(memo metadata);

    protected:
        ImagePipelineStepBase::memo data() const {return this->m_metadata.get_child(this->key());}
    };

}
#endif // OPENIMAGE_H
