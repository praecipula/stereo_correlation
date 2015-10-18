
#include "qimagecanvaswidget.h"

QImageCanvasWidget::QImageCanvasWidget(QWidget * parent, Qt::WindowFlags f) :
    QOpenGLWidget(parent, f)
{
    this->m_filePath = filename_ptr(new std::string(""));
}

QImageCanvasWidget::filename_ptr QImageCanvasWidget::changeImageFilePath(filename_ptr newFilePath)
{
    filename_ptr oldFilePath = this->m_filePath;
    this->m_filePath = newFilePath;
    return oldFilePath;
}

void QImageCanvasWidget::initializeGL()
{
    initializeOpenGLFunctions();
}

void QImageCanvasWidget::resizeGL(int w, int h)
{
    //m_projection.setToIdentity();
    //m_projection.perspective(60.0f, w / float(h), 0.01f, 1000.0f);
}

void QImageCanvasWidget::paintGL()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

void QImageCanvasWidget::updateImage()
{
    qInfo() << "Got the update message";
}
