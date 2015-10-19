
#include <qImage>
#include "qimagecanvaswidget.h"

QImageCanvasWidget::QImageCanvasWidget(QWidget * parent, Qt::WindowFlags f) :
    QOpenGLWidget(parent, f), m_textureHandle()
{
    // Initialization
    this->m_filePath = filename_ptr(new std::string(""));

    connect(this, SIGNAL(imagePathChanged()), this, SLOT(updateImageTexture()));
}

QImageCanvasWidget::filename_ptr QImageCanvasWidget::changeImageFilePath(filename_ptr newFilePath)
{
    filename_ptr oldFilePath = this->m_filePath;
    this->m_filePath = newFilePath;
    emit imagePathChanged();
    return oldFilePath;
}

void QImageCanvasWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_TEXTURE_2D);
}

void QImageCanvasWidget::resizeGL(int w, int h)
{
    //m_projection.setToIdentity();
    //m_projection.perspective(60.0f, w / float(h), 0.01f, 1000.0f);
}

void QImageCanvasWidget::paintGL()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1000, 0, 1000, 1000, -1000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    renderTextureCommands();

}

void QImageCanvasWidget::renderTextureCommands()
{
    if (m_textureHandle) {
        m_textureHandle->bind();
        glBegin(GL_QUADS);
        glTexCoord2d(0.0,0.0); glVertex2d(0.0,0.0);
        glTexCoord2d(1.0,0.0); glVertex2d(1000.0, 0.0);
        glTexCoord2d(1.0,1.0); glVertex2d(1000.0,1000.0);
        glTexCoord2d(0.0,1.0); glVertex2d(0.0,1000.0);
        glEnd();
    }
}

void QImageCanvasWidget::updateImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/Users/matt", tr("Image Files (*.png *.jpg *.bmp)"));
    filename_ptr new_filename = filename_ptr(new std::string(fileName.toUtf8().constData()));
    this->changeImageFilePath(new_filename);
}

void QImageCanvasWidget::updateImageTexture()
{
    // convert image to texture
    m_textureHandle = texture_ptr(new QOpenGLTexture(QImage(QString::fromStdString(*m_filePath)).mirrored()));
}
