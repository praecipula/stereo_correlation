#include "stereocorrelation.h"
#include "ui_stereocorrelation.h"


StereoCorrelation::StereoCorrelation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StereoCorrelation)
{
    QString appdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    Stereo::initialize();
    ui->setupUi(this);

    //wire up the left and right image canvases to respond to each other
    QLeftImageCanvasWidget* leftImageCanvas = this->findChild<QLeftImageCanvasWidget*>("leftImageCanvas");
    QRightImageCanvasWidget* rightImageCanvas = this->findChild<QRightImageCanvasWidget*>("rightImageCanvas");
    leftImageCanvas->registerSiblingWidget(rightImageCanvas);
    rightImageCanvas->registerSiblingWidget(leftImageCanvas);


    LOGI << "Application initialized";
 
}

StereoCorrelation::~StereoCorrelation()
{
    LOGI << "Destructing main form";
    delete ui;
}