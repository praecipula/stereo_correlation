#ifndef QRIGHTIMAGECANVASWIDGET
#define QRIGHTIMAGECANVASWIDGET

#include "qimagecanvaswidget.h"

namespace Stereo
{

  class QRightImageCanvasWidget: public QImageCanvasWidget
  {
  public:
    QRightImageCanvasWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

    virtual const std::string imageTextPrefix(){return "right";}
  };

}
#endif // QRIGHTIMAGECANVASWIDGET

