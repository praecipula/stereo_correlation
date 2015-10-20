
#include <qImage>
#include "qimagecanvaswidget.h"
#include <QWheelEvent>

QImageCanvasWidget::QImageCanvasWidget(QWidget * parent, Qt::WindowFlags f) :
    QOpenGLWidget(parent, f), m_textureHandle(), m_dimensions(), m_imageCamera()
{
    // Initialization
    this->m_filePath = filename_ptr(new std::string(""));

    // When the image path changes in this class for whatever reason, update the texture
    connect(this, SIGNAL(imagePathChanged()), this, SLOT(updateImageTexture()));
    // When the texture updates, render
    connect(this, SIGNAL(imageTextureChanged()), this, SLOT(update()));
}

QImageCanvasWidget::~QImageCanvasWidget()
{
  m_textureHandle.reset();
  m_filePath.reset();
}

void QImageCanvasWidget::registerSiblingWidget(QImageCanvasWidget *otherWidget)
{
    //When this sibling changes, the other sibling gets signals.
    connect(this, SIGNAL(cameraModelChanged(ImageCamera2d)), otherWidget, SLOT(updateCameraModel(ImageCamera2d)));
}

void QImageCanvasWidget::updateCameraModel(ImageCamera2d otherCamera)
{
    LOGD << "Got camera update: " << m_imageCamera.logFriendly() << " to " << otherCamera.logFriendly();
    m_imageCamera = otherCamera;
    // We're responsible for updating after camera changes
    update();
}

QImageCanvasWidget::filename_ptr QImageCanvasWidget::changeImageFilePath(filename_ptr newFilePath)
{
    filename_ptr oldFilePath = this->m_filePath;
    this->m_filePath = newFilePath;
    emit imagePathChanged();
    return oldFilePath;
}

void QImageCanvasWidget::wheelEvent(QWheelEvent* event)
{
    event->delta() > 0 ? m_imageCamera.increaseZoom() : m_imageCamera.decreaseZoom();
    emit cameraModelChanged(m_imageCamera);
    update();
}

void QImageCanvasWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_TEXTURE_2D);
}

void QImageCanvasWidget::resizeGL(int w, int h)
{
    // this window size changed
    glViewport(0, 0, w, h);

    // and the camera needs to know the new size as well
    m_imageCamera.adjustViewport(w, h);

}

void QImageCanvasWidget::paintGL()
{
    m_imageCamera.executeGl();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_DEPTH_BUFFER_BIT);
    // Red is pretty obvious as a background if we draw incorrectly
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    renderTextureCommands();

}

void QImageCanvasWidget::renderTextureCommands()
{
    if (m_textureHandle) {
        m_textureHandle->bind();
        // The coordinate system we're using is bottom-left is 0,0, with
        // the image extending up and to the right.
        // The coordinates are defined to extend to the image dimensions.
        glBegin(GL_QUADS);
        glTexCoord2d(0.0,0.0); glVertex2d(0.0,0.0);
        glTexCoord2d(1.0,0.0); glVertex2d(m_dimensions.width, 0.0);
        glTexCoord2d(1.0,1.0); glVertex2d(m_dimensions.width,m_dimensions.height);
        glTexCoord2d(0.0,1.0); glVertex2d(0.0,m_dimensions.height);
        glEnd();
    }
}

void QImageCanvasWidget::updateImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/Users/matt", tr("Image Files (*.png *.jpg *.bmp)"));
    filename_ptr new_filename = filename_ptr(new std::string(fileName.toUtf8().constData()));
    LOGD << imageTextPrefix() << " image changed to " << *new_filename;
    this->changeImageFilePath(new_filename);
}

void QImageCanvasWidget::updateImageTexture()
{
    // Build a QImage from the filename
    QImage image = QImage(QString::fromStdString(*m_filePath));
    // Pluck out its dimensions for our use
    m_dimensions.width = image.width();
    m_dimensions.height = image.height();
    // set the texture handle. We're using a smart ptr to QOpenGLTexture, which is not
    // the same thing as an opengl texture handle, but we'll let QOpenGLTexture
    // handle the conversion from image to texture bytes registered with
    // glBindTexture
    m_textureHandle = texture_ptr(new QOpenGLTexture(image.mirrored()));
    // Reset the camera
    m_imageCamera.adjustCenter(image.width() / 2, image.height() / 2);
    // This is a bit hacky.
    resizeGL(this->width(), this->height());
    update();
    emit imageTextureChanged();
}
