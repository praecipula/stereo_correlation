#ifndef RETICLE_H
#define RETICLE_H


#include <QMetaType>
#include <QColor>
#include <QPoint>
#include <QOpenGLTexture>
#include "boost/shared_ptr.hpp"

class Reticle
{
public:
    static const QPoint s_noPoint;
    static const QPoint s_hotSpot;
    typedef boost::shared_ptr<QOpenGLTexture> texture_ptr;

    Reticle();
    void initializeTexture();

    void moveTo(const QPoint& coords);
    void paintGL();

private:
    QPoint m_location;
    QPoint m_dimensions;
    QColor m_color;
    texture_ptr m_texture;
};

Q_DECLARE_METATYPE(Reticle)

#endif // RETICLE_H
