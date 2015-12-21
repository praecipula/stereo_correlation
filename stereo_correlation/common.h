#ifndef STEREO_COMMON
#define STEREO_COMMON



#include <QStandardPaths>
#include <QDir>

/*
 * Sigh. Qt defines some helpful keyword-like macros, such as "signals" and "slots" and
 * "emit". Unfortunately, it also defines "foreach". This is a massive collision for
 * OpenVDB, which defines its own openvdb::foreach. The culprit here is really Qt, for
 * going "above and beyond" what it needs to in macro definitions, without having fine
 * grained switches for avoiding the collision - you can disable all keywords or none.
 * I want to keep the "signals" and "slots" keywords, but not "foreach"
 */

#ifndef STEREO_CORRELATION_UNDEFS
#define STEREO_CORRELATION_UNDEFS

#undef foreach

#endif

#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include "metatyperegistration.h"
#include "model/stereomesh.h"

namespace Stereo
{

    using namespace std;



inline void initialize()
{
    QString appdata = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (!QDir(appdata).exists())
    {
        QDir().mkdir(appdata);
        //... why did I do this in this if statement? Is this a bug?
        MetatypeRegistration reg;
        reg.doRegistration();
    }
    // Initialize our stereo mesh and related types
    StereoMesh::initialize();
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

