#include "qcomputationaldagwidget.h"

namespace Stereo
{

    QComputationalDagWidget::QComputationalDagWidget(QWidget * parent, Qt::WindowFlags f):
        QOpenGLWidget(parent, f), m_imageCamera()
    {}

    QComputationalDagWidget::~QComputationalDagWidget()
    {}

    void QComputationalDagWidget::initializeGL()
    {
        initializeOpenGLFunctions();
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void QComputationalDagWidget::resizeGL(int w, int h)
    {
        // this window size changed
        glViewport(0, 0, w, h);

        // and the camera needs to know the new size as well
        m_imageCamera.adjustViewport(w, h);

    }

    void QComputationalDagWidget::paintGL()
    {
        m_imageCamera.executeGl();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glClear(GL_DEPTH_BUFFER_BIT);
        // Red is pretty obvious as a background if we draw incorrectly
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    }


}
