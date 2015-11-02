
#include <qImage>
#include "qimagecanvaswidget.h"
#include <QWheelEvent>
#include "strcon.h"
#include <Magick++/Image.h>
#include "saveimage.h"


namespace Stereo
{

    const QPoint QImageCanvasWidget::s_noPoint(-1, -1);

    QImageCanvasWidget::QImageCanvasWidget(QWidget * parent, Qt::WindowFlags f) :
      QOpenGLWidget(parent, f), m_textureHandle(), m_dimensions(), m_imageCamera(), m_reticle(),
      m_mousePressLocation(s_noPoint), m_imageCenterLocation(s_noPoint)
    {
      // Initialization
      this->m_filePath = filename_ptr(new std::string(""));

      // When the image path changes in this class for whatever reason, update the texture
      connect(this, SIGNAL(imagePathChanged()), this, SLOT(updateImageTexture()));
      // When the texture updates, render
      connect(this, SIGNAL(imageTextureChanged()), this, SLOT(update()));
      // There is no cursor on this widget (well, we define the cursor as the reticle)
      setCursor(Qt::BlankCursor);
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
      connect(this, SIGNAL(reticleLocationChanged(Reticle)), otherWidget, SLOT(updateReticleLocation(Reticle)));
    }

    void QImageCanvasWidget::updateCameraModel(ImageCamera2d otherCamera)
    {
      LOGD << "Got camera update: " << m_imageCamera.logFriendly() << " to " << otherCamera.logFriendly();
      m_imageCamera = otherCamera;
      // We're responsible for updating after camera changes
      update();
    }

    void QImageCanvasWidget::updateReticleLocation(Reticle desiredReticle)
    {

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

    void QImageCanvasWidget::mousePressEvent(QMouseEvent *event)
    {
      // Put the location of the mouse press in the mouse press location variable
      LOGV << "Mouse click in image canvas widget: " << event->x() << 'x' << event->y();
      m_mousePressLocation = event->pos();
      m_imageCenterLocation = m_imageCamera.getCenter();
    }

    void QImageCanvasWidget::mouseReleaseEvent(QMouseEvent *event)
    {
      // unset the mouse press location
      LOGV << "Mouse released in image canvas widget";
      m_mousePressLocation = s_noPoint;
    }

    void QImageCanvasWidget::mouseMoveEvent(QMouseEvent *event)
    {
      // If we're in a pressed state
      if (m_mousePressLocation != s_noPoint)
      {
        // get the scaled distance since press
        QPoint delta(QPoint(event->x(), event->y()) - m_mousePressLocation);
        QPoint scaledDelta(m_imageCamera.calculateScaledDelta(delta));
        // We need to convert from screen coordinates (x+right, y+down) to OpenGL coordinates (x+right, y+up)
        // But there's a wrinkle! We would be then telling the camera to move in the same location as the mouse.
        // Think of a jeweler's loupe or a microscope moving across the surface: if we went in the same direction,
        // we'd be moving the viewer and not the image. We want the feeling of picking up and moving the image.
        // Therefore, we double-invert: we want the inverse sense of X and Y in OpenGL to move the camera such
        // that the image appears to move correctly.
        QPoint coordinateCorrectDelta = QPoint(-scaledDelta.x(), scaledDelta.y());
        QPoint centerpoint(m_imageCenterLocation + coordinateCorrectDelta);
        LOGV << "Click and drag: from " << StrCon(m_mousePressLocation) << " to " << StrCon(event->pos()) << " (scaled) " << StrCon(scaledDelta);
        m_imageCamera.adjustCenter(centerpoint.x(), centerpoint.y());
        emit cameraModelChanged(m_imageCamera);
        update();
      }

      // Correlate the center of the widget to the center of the image
      QPoint widgetCenter(width() / 2, height() / 2);
      // The delta in screen coordinates from center
      QPoint delta(QPoint(event->x(), event->y()) - widgetCenter);
      // The delta in modelspace from center
      QPoint scaledDelta(m_imageCamera.calculateScaledDelta(delta));
      // With correct coordinate flipping
      QPoint coordinateCorrectDelta = QPoint(scaledDelta.x(), -scaledDelta.y());
      //
      QPoint reticlePoint(m_imageCamera.getCenter() + coordinateCorrectDelta);
      m_reticle.moveTo(reticlePoint);
      LOGV << "Updated";
      update();

    }

    void QImageCanvasWidget::initializeGL()
    {
      initializeOpenGLFunctions();
      glEnable(GL_TEXTURE_2D);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      m_reticle.initializeTexture();
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
      m_reticle.paintGL();

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
      update();
      emit imageTextureChanged();
    }
}
