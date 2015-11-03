#ifndef IMAGEPROCESSINGVIEW_H
#define IMAGEPROCESSINGVIEW_H

#include <QMainWindow>
#include <vector>
#include "image_pipeline/imagepipelinestepbase.h"

#include "image_pipeline/guiimagepipelinebuilder.h"

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
    // A mapping from a string (deprecated: should be widget), such as Open Image, to the
    // guiimagepipelinebuilder function that constructs that step.
    typedef std::map<std::string, GuiImagePipelineBuilder::fn_ptr> string_mapping;

    explicit ImageProcessingView(QWidget *parent = 0);
    ~ImageProcessingView();

    void populateAvailableSteps();


  private:
    Ui::ImageProcessingView *ui;
    processing_steps m_stepsToExecute;
    processing_steps m_stepsAvailable;
    string_mapping m_stringMapping;
    GuiImagePipelineBuilder m_stepBuilder;
  };

}

#endif // IMAGEPROCESSINGVIEW_H
