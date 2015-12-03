#ifndef STEREO_COMMON
#define STEREO_COMMON

#include <QStandardPaths>
#include <QDir>
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include "metatyperegistration.h"

namespace Stereo
{

    using namespace std;

inline void initialize()
{
    QString appdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir(appdata).exists())
    {
        QDir().mkdir(appdata);
        MetatypeRegistration reg;
        reg.doRegistration();
    }
#ifdef STEREO_TEST
#pragma message "Console appender"
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
    plog::init(plog::verbose, &consoleAppender);
#else
    plog::init(plog::verbose,
               (appdata + "/log.txt").toStdString().data());
#endif
}

}

#endif // STEREO_COMMON

