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
      OpenImage(const std::string& imageFilename);
      OpenImage(const OpenImage& other);
      virtual ~OpenImage() {}

      void set_filename(const std::string& imageFilename);

      virtual image_list execute(const image_list &inputs);
      std::string m_imageFilename;
    };

}
#endif // OPENIMAGE_H
