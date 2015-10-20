#include "stereocorrelation.h"
#include <QApplication>
#include "common.h"

#include "metatype_includes.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StereoCorrelation w;
    w.show();

    // Our metatypes
    qRegisterMetaType<ImageCamera2d>();

    return a.exec();
}
