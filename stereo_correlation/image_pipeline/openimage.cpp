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


  ImagePipelineStepBase::DataList OpenImage::execute(const ImagePipelineStepBase::DataList& inputs)
  {
      //TODO: do we assert that inputs is empty here?
      ImagePipelineStepBase::DataList images;
      // Open the image and check for success
      Algo::ImageBase::ptr image = Algo::ImageBase::load(m_imageFilename);
      std::stringstream sstr;
      sstr << "Could not load image " << m_imageFilename;
      STEREO_LOG_ASSERT(!image->empty(), sstr.str());
      // Set the image in the data
      ImagePipelineStepBase::ImagePipelineData data;
      data.image = image;
      // Metadata: the filename that was given to us
      ImagePipelineStepBase::memo metadata;
      metadata.put("filename", m_imageFilename);
      metadata.put("version", 1);
      // For double-checking sake, calculate a checksum.
      // This can be used to "fingerprint" a file later to be sure it was the same one.
      metadata.put("checksum", data.image->crc());
      // Metadata goes under "open_image" key
      data.metadata.add_child("open_image", metadata);
      // There's one and only one data in the list.
      images.push_back(data);
      return images;
  }
}
