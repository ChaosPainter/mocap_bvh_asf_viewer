#ifndef QWIDGETGL_H
#define QWIDGETGL_H


#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include "vertex.h"
#include "transform3d.h"
#include <vector>
#include "camera3d.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include "input.h"

class QExposeEvent;
class QOpenGLShaderProgram;
using namespace std;


class Qwidgetgl : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
  public:
    Qwidgetgl(QWidget *parent = 0);
    ~Qwidgetgl();
    // Create a colored triangle

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void teardownGL();
  public slots:
    void refresh(QVector<Vertex> array,int mode);
    void update();
    void right_click(const QPoint&);

public:
    // OpenGL State Information
    QOpenGLBuffer m_vertex;
    QOpenGLVertexArrayObject m_object;
    QOpenGLShaderProgram *m_program;

    int u_worldToCamera;
    int u_cameraToView;
    int u_modelToWorld;
    int u_worldToView;
    int last_camera_angle;
    QMatrix4x4 m_projection;
    Camera3D m_camera;
    Transform3D m_transform;
    QVector<Vertex> sg_vertex;
    bool lines=false;
    bool cylinders=false;
    bool cube=true;
    
    QString flaga;
    // Private Helpers
    void printContextInformation();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);


};

#endif // QWIDGETGL_H
