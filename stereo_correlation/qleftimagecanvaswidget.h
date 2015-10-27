#ifndef QLEFTIMAGECANVASWIDGET
#define QLEFTIMAGECANVASWIDGET

#include "qimagecanvaswidget.h"
#include "strcon.h"

class QLeftImageCanvasWidget: public QImageCanvasWidget
{
    Q_OBJECT
public:
    QLeftImageCanvasWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    virtual ~QLeftImageCanvasWidget(){}

    virtual const std::string imageTextPrefix(){return StrCon(tr("left"));}

    virtual filename_ptr changeImageFilePath(filename_ptr newFilePath);

signals:
    void leftImageFilenameChanged(QString newFilename);

};

#endif // QLEFTIMAGECANVASWIDGET

