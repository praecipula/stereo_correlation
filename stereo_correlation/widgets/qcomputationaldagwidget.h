#ifndef QCOMPUTATIONALDAGWIDGET_H
#define QCOMPUTATIONALDAGWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include "imagecamera2d.h"

namespace Stereo
{
    /*
     * Qt has no classes capable of displaying a dag. We could use graphviz, but
     * it's not interactive.
     *
     * Sometimes a guy's just gotta do the job himself.
     */
    class QComputationalDagWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        QComputationalDagWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
        ~QComputationalDagWidget();

        /********
        * OpenGL hooks
        ********/
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();

    private:
        ImageCamera2d m_imageCamera;

    };
}
#endif // QCOMPUTATIONALDAGWIDGET_H
