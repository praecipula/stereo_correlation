#ifndef IMAGECAMERA2D_H
#define IMAGECAMERA2D_H

#include "common.h"
#include <boost/array.hpp>
#include <QOpenGLFunctions>

/*
 * The ImageCamera2d class is responsible for mapping image coordinates to
 * scene coordinates: how to locate the object in the scene, translate
 * coordinates to and from scene coordinates, and manage zoom/scale
 * operations
 */
class ImageCamera2d : protected QOpenGLFunctions
{
private:
    enum {MAX_STOPS=8};
public:

    static const float s_scaleStops[MAX_STOPS];

    ImageCamera2d();
    ImageCamera2d(const ImageCamera2d& other);
    virtual ~ImageCamera2d();

    ImageCamera2d& operator=(const ImageCamera2d& other);

    // Execute the opengl commands to position the scene.
    void executeGl();

    void adjustViewport(int newWidth, int newHeight);
    void getViewport(int& width, int& height){width = m_viewport[0], height = m_viewport[1];}
    void adjustCenter(int newCenterX, int newCenterY);

    void increaseZoom(){setZoomStop(m_scaleStop + 1);}
    void decreaseZoom(){setZoomStop(m_scaleStop - 1);}
    void setZoomStop(int stop){int c_stop = stop < 0 ? 0 : stop > MAX_STOPS -1 ? MAX_STOPS - 1 : stop;
      LOGD << "Setting zoom factor to " << s_scaleStops[c_stop];
      m_scaleStop = c_stop;
    }

    std::string logFriendly(){std::stringstream buf;
                              buf << "2d camera: zoom " << s_scaleStops[m_scaleStop] << ", " << m_viewport[0] << "x" << m_viewport[1] <<
                                     "@" << m_center[0] << "," << m_center[1];
                              return buf.str(); }

private:
    // this controls zoom in and out.
    // There are discrete stops assigned.
    int m_scaleStop;
    boost::array<int, 2> m_center;
    boost::array<int, 2> m_viewport;
};

Q_DECLARE_METATYPE(ImageCamera2d)

#endif // IMAGECAMERA2D_H