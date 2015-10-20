#include "imagecamera2d.h"

const float ImageCamera2d::s_scaleStops[] = {0.125, 0.25, 0.5, 1, 2, 4, 8, 16};

ImageCamera2d::ImageCamera2d():
    m_scaleStop(2), m_center(), m_viewport()
{
}

ImageCamera2d::ImageCamera2d(const ImageCamera2d &other):
    m_scaleStop(other.m_scaleStop), m_center(other.m_center), m_viewport(other.m_viewport)
{}

ImageCamera2d::~ImageCamera2d()
{}

ImageCamera2d& ImageCamera2d::operator=(const ImageCamera2d& other)
{
    m_scaleStop = other.m_scaleStop;
    m_center = other.m_center;
    m_viewport = other.m_viewport;
    return *this;
}

void ImageCamera2d::executeGl()
{
    //todo: do this in the camera
    int half_width = m_viewport[0] / 2;
    int half_height = m_viewport[1] / 2;
    float scale = s_scaleStops[m_scaleStop];
    // Set up the viewport. (do we have to do this every time, or just on resize?

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Set up the orthographic view with scaling
    // Note that we divide by the scaling; this is because the viewpoint, to take up
    // half the image, must span twice the coverage.
    glOrtho(m_center[0] - half_width / scale, m_center[0] + half_width / scale,
            m_center[1] - half_height / scale, m_center[1] + half_height / scale,
            1000, -1000);
}

void ImageCamera2d::adjustViewport(int newWidth, int newHeight)
{
    // When the viewport adjusts, we want to keep the center, but
    // simply increase the width and height.
    m_viewport[0] = newWidth;
    m_viewport[1] = newHeight;
    LOGD << "Image viewport changed to " << newWidth << "x" << newHeight;
}

void ImageCamera2d::adjustCenter(int newCenterX, int newCenterY)
{
    m_center[0] = newCenterX;
    m_center[1] = newCenterY;
    LOGD << "Image center changed to " << newCenterX << "x" << newCenterY;
}
