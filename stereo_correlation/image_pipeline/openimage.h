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
        static const std::string key;
        static const std::string version;

        OpenImage(const std::string& imageFilename);
        OpenImage(const memo& metadata);
        OpenImage(const OpenImage& other);
        virtual ~OpenImage() {}

        std::string filename() const;
        void set_filename(const std::string& imageFilename);

        virtual DataList execute(const DataList& inputs);
        static ImagePipelineStepBase::shared_ptr load(memo metadata);
    };

}
#endif // OPENIMAGE_H
