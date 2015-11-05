#ifndef RETICLE_H
#define RETICLE_H


#include <QMetaType>
#include <QColor>
#include <QOpenGLTexture>
#include <memory>
#include "model/coordinate.h"

namespace Stereo
{

  class Reticle
  {
  public:
    static const Coordinate s_hotSpot;
    typedef std::shared_ptr<QOpenGLTexture> texture_ptr;

    Reticle();
    void initializeTexture();

    void moveTo(const QPoint& coords);
    void paintGL();

  private:
    Coordinate m_location;
    Coordinate m_dimensions;
    QColor m_color;
    texture_ptr m_texture;
  };

}

Q_DECLARE_METATYPE(Stereo::Reticle)

#endif // RETICLE_H
