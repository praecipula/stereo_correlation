#include "imageprocessingview.h"
#include "ui_imageprocessingview.h"

// processing steps
#include "saveimage.h"

namespace Stereo
{
    ImageProcessingView::ImageProcessingView(QWidget *parent) :
      QMainWindow(parent),
      ui(new Ui::ImageProcessingView),
      m_stepsAvailable(),
      m_stepsToExecute()
    {
      ui->setupUi(this);
      // Add all the known processing steps to available steps
      // The user moves them into the execution queue.
      m_stepsAvailable.push_back(ImagePipelineStepBase::ptr(new SaveImage));

    }

    ImageProcessingView::~ImageProcessingView()
    {
      delete ui;
    }
}
