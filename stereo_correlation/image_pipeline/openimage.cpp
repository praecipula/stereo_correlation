#include "openimage.h"
#include <Magick++/Image.h>

namespace Stereo
{

  OpenImage::OpenImage(const std::string& imageFilename):
      ImagePipelineStepBase(), m_imageFilename(imageFilename)
  {}

  OpenImage::OpenImage(const OpenImage &other):
      ImagePipelineStepBase(), m_imageFilename(other.m_imageFilename)
  {}


  ImagePipelineStepBase::image_list OpenImage::execute(const ImagePipelineStepBase::image_list& inputs)
  {
      image_list images;
      images.push_back(image_ptr(new Magick::Image(m_imageFilename)));
      return images;
  }
}
