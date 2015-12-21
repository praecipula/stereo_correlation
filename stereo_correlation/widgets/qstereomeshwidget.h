#ifndef QSTEREOMESHWIDGET_H
#define QSTEREOMESHWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

namespace Stereo
{
    class QStereoMeshWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT
    public:
        QStereoMeshWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
        virtual ~QStereoMeshWidget();



        void initializeGL(){}
        void resizeGL(int w, int h){}
        void paintGL(){}

    private:
        QImage m_leftImage;

    signals:

    public slots:
    };
}
#endif // QSTEREOMESHWIDGET_H
