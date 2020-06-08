#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <GL/GLU.h>
#include <GL/GL.h>
#include <QWidget>
#include <QOpenGLWidget>


class OpenGLWidget : public QOpenGLWidget
{
public:
    OpenGLWidget(QWidget *parent = 0);
    ~OpenGLWidget();
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // OPENGLWIDGET_H
