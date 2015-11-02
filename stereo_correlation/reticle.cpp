
#include "reticle.h"
#include "common.h"
#include <QCoreApplication>

namespace Stereo
{

    const QPoint Reticle::s_noPoint(-1, -1);
    const QPoint Reticle::s_hotSpot(20, 20);

    Reticle::Reticle():
      m_location(s_noPoint), m_dimensions(s_noPoint), m_color(255, 255, 255, 255), m_texture()
    {
    }

    void Reticle::initializeTexture()
    {
      QString resourceImage = QCoreApplication::applicationDirPath() + "/../Resources/reticle.png";
      QImage image = QImage(resourceImage);
      m_texture = texture_ptr(new QOpenGLTexture(image.mirrored()));
      m_dimensions = QPoint(image.width(), image.height());
    }

    void Reticle::moveTo(const QPoint &coords)
    {
      m_location = coords;
    }

    void Reticle::paintGL()
    {
      /* The x,y coordinate of the reticle is stored in the location point,
     * but needs an offset in order to account for the fact that the hot spot is
     * not at the bottom-left of the texture.
     * The raw location would paint the bottom left coordinate, so we shift the
     * texture down and to the left - negatively - based on the hot spot location.
     */
      QPoint glCoordinates = m_location - s_hotSpot;
      if (m_texture) {
        m_texture->bind();
        glPushMatrix();
        glLoadIdentity();
        glTranslated(glCoordinates.x(), glCoordinates.y(), 1);
        // The coordinate system we're using is bottom-left is 0,0, with
        // the image extending up and to the right.
        // The coordinates are defined to extend to the image dimensions.
        glBegin(GL_QUADS);
        glTexCoord2d(0.0,0.0); glVertex2d(0.0,0.0);
        glTexCoord2d(1.0,0.0); glVertex2d(m_dimensions.x(), 0.0);
        glTexCoord2d(1.0,1.0); glVertex2d(m_dimensions.x(),m_dimensions.y());
        glTexCoord2d(0.0,1.0); glVertex2d(0.0,m_dimensions.y());
        glEnd();
        glPopMatrix();
      }

    }

}
