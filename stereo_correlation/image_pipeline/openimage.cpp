#include <sstream>
#include "openimage.h"
#include "algorithm/stereo/imagebase.h"
#include "stereoexception.h"

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
      Algo::ImageBase::ptr image = Algo::ImageBase::load(m_imageFilename);
      std::stringstream sstr;
      sstr << "Could not load image " << m_imageFilename;
      STEREO_LOG_ASSERT(!image->empty(), sstr.str());
      images.push_back(image);
      return images;
  }
}
