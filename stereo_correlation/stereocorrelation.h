#ifndef STEREOCORRELATION_H
#define STEREOCORRELATION_H

#include <QMainWindow>

namespace Ui {
class StereoCorrelation;
}

class StereoCorrelation : public QMainWindow
{
    Q_OBJECT

public:
    explicit StereoCorrelation(QWidget *parent = 0);
    ~StereoCorrelation();

private:
    Ui::StereoCorrelation *ui;
};

#endif // STEREOCORRELATION_H
