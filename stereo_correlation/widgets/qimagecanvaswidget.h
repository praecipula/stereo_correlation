#ifndef QIMAGECANVASWIDGET
#define QIMAGECANVASWIDGET

#include "common.h"
#include <boost/smart_ptr.hpp>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QFileDialog>

#include "imagecamera2d.h"
#include "reticle.h"

namespace Stereo
{

    /*
 * The image canvas widget is the controller for all things to do with
 * the image displaying of the individual pictures making up the stereo
 * picture. It controls not only the rendering of the scene, but also
 * acts as a mediator between multiple helper classes which handle certain
 * aspects of the display and management of these images.
 * As a rule of thumb, all signals go out of this class and all slots come
 * into it, and it delegates to its helpers, rather than connecting to
 * the helpers themselves. (Law of Demeter in action).
 */
    class QImageCanvasWidget: public QOpenGLWidget, protected QOpenGLFunctions
    {
      Q_OBJECT
      /********
     * Typedefs
     ********/
    public:
      typedef boost::shared_ptr<std::string> filename_ptr;
      typedef boost::shared_ptr<QOpenGLTexture> texture_ptr;
      // Sentinel value. Ehh.
      static const QPoint s_noPoint;

      /********
     * Object functions
     ********/
    public:
      QImageCanvasWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
      ~QImageCanvasWidget();

      // Each canvas is linked and responds to the other
      void registerSiblingWidget(QImageCanvasWidget* otherWidget);

      // A method to programmatically change the file path of the image
      virtual filename_ptr changeImageFilePath(filename_ptr newFilePath);

      // Function to take changed filename and update the image texture for OpenGL


      /* This is a prefix used when inferring things about subclasses, e.g. when
     * munging or navigating strings, which we handle up here, but we need to
     * handle differently depending on the base class' value. It has a prefix
     * it can supply to us.
     */
      virtual const std::string imageTextPrefix() = 0;

      /********
     * OpenGL hooks
     ********/
      void initializeGL();
      void resizeGL(int w, int h);
      void paintGL();
      void renderTextureCommands();

      /********
     * Overrides
     *******/
      // On the mouse wheel, we zoom in and out with the camera
      void wheelEvent(QWheelEvent* event);
      // On mouse press, we begin dragging the camera to pan
      void mousePressEvent(QMouseEvent* event);
      // On mouse release, we stop dragging and update the sibling camera
      void mouseReleaseEvent(QMouseEvent* event);
      // On mouse move, we update a cursur/reticle if appropriate.
      void mouseMoveEvent(QMouseEvent* event);

      /*******
     * Signals and slots
     ********/
    public slots:
      // This is called from the menu or the button to change the image with QFileDialog.
      void updateImage();
      // This is connected to update the in-memory opengl texture handle when the image filename changed
      void updateImageTexture();
      // Recieved when the sibling widget updates its camera model zoom.
      // This means we need to register the ImageCamera, or at least a params class,
      // with the metaclass system.
      void updateCameraModel(ImageCamera2d desiredCameraModel);
      void updateReticleLocation(Reticle desiredReticle);
    signals:
      void imagePathChanged();
      void imageTextureChanged();
      void cameraModelChanged(ImageCamera2d changed);
      void reticleLocationChanged(Reticle changed);

      /********
     * Member variables
     ********/
    protected:

      filename_ptr m_filePath;
      texture_ptr m_textureHandle;
      struct image_dimensions {
        int width;
        int height;
      };

      image_dimensions m_dimensions;
      ImageCamera2d m_imageCamera;
      Reticle m_reticle;
      // The screen coordinates on mouse click
      QPoint m_mousePressLocation;
      // The image coordinates on mouse click
      QPoint m_imageCenterLocation;

    };

}
#endif // QIMAGECANVASWIDGET
