#include "stereocorrelation.h"
#include "ui_stereocorrelation.h"

StereoCorrelation::StereoCorrelation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StereoCorrelation)
{
    ui->setupUi(this);
}

StereoCorrelation::~StereoCorrelation()
{
    delete ui;
}
