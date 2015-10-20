#include "stereocorrelation.h"
#include <QApplication>
#include "common.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StereoCorrelation w;
    w.show();



    return a.exec();
}
