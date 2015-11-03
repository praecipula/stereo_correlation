#include "saveimage.h"
#include <Magick++/Image.h>

#include <QFileDialog>
#include "strcon.h"

namespace Stereo
{
    SaveImage::SaveImage(const std::string filename):
      ImagePipelineStepBase(), m_filename(filename)
    {}

    SaveImage::SaveImage(const SaveImage &other):
        ImagePipelineStepBase(), m_filename(other.m_filename)
    {}

    /*
 * Save the image at m_filename, and then return an empty image_list.
 * If m_filename is unset, this will prompt the user for a filename.
 */
    ImagePipelineStepBase::image_list SaveImage::execute(const ImagePipelineStepBase::image_list &inputs)
    {
      if (m_filename.empty())
      {
        promptForFilename();
      }
      if (m_filename.empty())
      {
        LOGW << "Filename is empty, even after prompt. File not saving.";
        return image_list();
      }
      inputs.front()->write(m_filename);
      return image_list();
    }

    void SaveImage::promptForFilename()
    {
      QString fileName = QFileDialog::getOpenFileName(NULL,
                                                      QObject::tr("Choose destination image filename"), "/Users/matt", QObject::tr("Image Files (*.png *.jpg *.bmp)"));
      m_filename = StrCon(fileName);
      LOGD << "Save image location changed to " << m_filename;
    }
}
