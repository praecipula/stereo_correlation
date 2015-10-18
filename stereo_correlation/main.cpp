#include "stereocorrelation.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StereoCorrelation w;
    w.show();

    return a.exec();
}
