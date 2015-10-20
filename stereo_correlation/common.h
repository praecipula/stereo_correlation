#ifndef STEREO_COMMON
#define STEREO_COMMON

#include <QStandardPaths>
#include <QDir>
#include <plog/Log.h>

namespace Stereo
{
inline void initialize()
{
    QString appdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir(appdata).exists())
    {
        QDir().mkdir(appdata);
    }
    plog::init(plog::debug,
               (appdata + "/log.txt").toStdString().data());
}

}

#endif // STEREO_COMMON

