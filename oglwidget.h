
#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <gl/GLU.h>
#include <gl/GL.h>
#include <QOpenGLFunctions>

class OGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    OGLWidget(QWidget *parent = 0);
    ~OGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif // OGLWIDGET_H
