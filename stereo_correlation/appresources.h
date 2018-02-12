#ifndef APPRESOURCES_H
#define APPRESOURCES_H

#include <QFile>
#include <QString>
#include <QDir>
#include <QCoreApplication>

#include "strcon.h"

namespace Stereo
{
    /**
     * Wrapper for functions that return locations / files that represent resources that are app specific; in this
     * way we can keep a cross-platform location library that just returns files and such when needed.
     */
    namespace AppResources
    {
        using namespace std;

        /**
         * @brief getBundleResourcesPath
         * @return the path to the bundled or installed resources directory.
         */
        string getBundleResourcesPath()
        {
            //TODO: QT_CLI
            #if defined(Q_OS_WIN)
            return StrCon(QCoreApplication::applicationDirPath() + "/").to_s();
            #elif defined(Q_OS_OSX)
            return StrCon(QCoreApplication::applicationDirPath() + "/../Resources/").to_s();
            #elif defined(Q_OS_LINUX)
            return StrCon(QCoreApplication::applicationDirPath() + "/../share/stereo_correlation/").to_s();
            #else
            return StrCon(QCoreApplication::applicationDirPath() + "/").to_s();
            #endif
        }

        string cameraCalibrationPath(string calibrationName)
        {
            return getBundleResourcesPath() + "camera_calibrations/" + calibrationName + ".json";
        }
    }
}

#endif // APPRESOURCES_H
