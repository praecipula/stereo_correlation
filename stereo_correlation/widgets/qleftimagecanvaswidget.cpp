
#include "qleftimagecanvaswidget.h"
#include "strcon.h"


namespace Stereo
{

    QLeftImageCanvasWidget::QLeftImageCanvasWidget(QWidget * parent, Qt::WindowFlags f) :
      QImageCanvasWidget(parent, f)
    {
    }

    QImageCanvasWidget::filename_ptr QLeftImageCanvasWidget::changeImageFilePath(filename_ptr newFilePath)
    {
      // Call super
      QImageCanvasWidget::filename_ptr old_ptr(QImageCanvasWidget::changeImageFilePath(newFilePath));
      emit leftImageFilenameChanged(StrCon(*newFilePath));
      return old_ptr;
    }
}
