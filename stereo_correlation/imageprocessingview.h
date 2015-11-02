#ifndef IMAGEPROCESSINGVIEW_H
#define IMAGEPROCESSINGVIEW_H

#include <QMainWindow>
#include <vector>
#include "imagepipelinestepbase.h"

namespace Ui {
  class ImageProcessingView;
}

namespace Stereo
{

  class ImageProcessingView : public QMainWindow
  {
    Q_OBJECT

  public:

    typedef std::vector<ImagePipelineStepBase::ptr> processing_steps;

    explicit ImageProcessingView(QWidget *parent = 0);
    ~ImageProcessingView();

  private:
    Ui::ImageProcessingView *ui;
    processing_steps m_stepsToExecute;
    processing_steps m_stepsAvailable;
  };

}

#endif // IMAGEPROCESSINGVIEW_H
