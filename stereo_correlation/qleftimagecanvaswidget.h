#ifndef QLEFTIMAGECANVASWIDGET
#define QLEFTIMAGECANVASWIDGET

#include "qimagecanvaswidget.h"

class QLeftImageCanvasWidget: public QImageCanvasWidget
{
public:
    QLeftImageCanvasWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

    virtual const std::string imageTextPrefix(){return "left";}
};

#endif // QLEFTIMAGECANVASWIDGET

