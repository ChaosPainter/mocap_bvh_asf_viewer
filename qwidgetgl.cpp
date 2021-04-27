#include "qwidgetgl.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include "vertex.h"
#include "input.h"




Qwidgetgl::Qwidgetgl(QWidget *parent): QOpenGLWidget(parent)
{


    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
   m_transform.translate(0.0f, 0.0f, 0.0f);//

   QSurfaceFormat format;
   format.setDepthBufferSize(24);
   format.setStencilBufferSize(8);
     format.setVersion(3, 3);
   format.setProfile(QSurfaceFormat::CoreProfile);
   this->setFormat(format);
}

Qwidgetgl::~Qwidgetgl()
{
    makeCurrent();
    teardownGL();
}

void Qwidgetgl::initializeGL()
{

    initializeOpenGLFunctions();
    {

      m_program = new QOpenGLShaderProgram();
      m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.vert");
      m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.frag");
      m_program->link();
      m_program->bind();

          u_modelToWorld = m_program->uniformLocation("modelToWorld");//
          u_worldToCamera = m_program->uniformLocation("worldToCamera");
          u_cameraToView = m_program->uniformLocation("cameraToView");

      m_vertex.create();
      m_vertex.bind();
      m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
      m_vertex.allocate(sg_vertex.constData(), sg_vertex.count()*sizeof(sg_vertex[0]));

      m_object.create();
      m_object.bind();

      m_program->enableAttributeArray(0);
      m_program->enableAttributeArray(1);
      m_program->setAttributeBuffer(0, GL_FLOAT,offsetof(Vertex, m_position),
                                    //Vertex::positionOffset(),
                                    Vertex::PositionTupleSize,
                                    sizeof(Vertex));
      m_program->setAttributeBuffer(1, GL_FLOAT,offsetof(Vertex, m_color), Vertex::ColorTupleSize, sizeof(Vertex));

      // Release (unbind) all
      m_object.release();
      m_vertex.release();
      m_program->release();


    }
    //qDebug()<<"initializeGl_2\n";
}

void Qwidgetgl::resizeGL(int width, int height)
{

    m_projection.setToIdentity();
    m_projection.perspective(45.0f, width / float(height), 0.0f, 1000.0f);//
    //qDebug()<<"resizeGl_1\n";
}

void Qwidgetgl::paintGL()
{
    glEnable(GL_DEPTH_TEST);

   glEnable(GL_CULL_FACE);



    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    QMatrix4x4 view;
    QMatrix4x4 projection;
    QMatrix4x4 model;
    view.lookAt(QVector3D(0, 0, 100),
                QVector3D(0, 0, 0),
                QVector3D(0, 1, 0));
    projection.perspective(45, 1, 1, 1000);

    view.translate(0, 0, 0);

    m_program->setUniformValue(u_worldToCamera, view*m_camera.toMatrix());
    m_program->setUniformValue(u_cameraToView, projection);
    {
      m_object.bind();
      m_program->setUniformValue(u_modelToWorld, model);
      if (lines)
      {
      glDrawArrays(GL_LINES, 0, sg_vertex.count());
      }
      else if (cylinders)
      {
      // kod do lini
          // kod do trójkątów
          glDrawArrays(GL_LINES, 0, 4);
          glDrawArrays(GL_TRIANGLES,4,sg_vertex.count()-4);

      }

      m_object.release();
    }
    m_program->release();
    //qDebug()<<"paintGl_2\n";




}

void Qwidgetgl::teardownGL()
{

    m_object.destroy();
    m_vertex.destroy();
    delete m_program;
}

void Qwidgetgl::refresh(QVector<Vertex> array,int mode)
{


    if(mode==0)
    {
   // qDebug()<<"refreshGl_1\n";
    sg_vertex=array;

    m_vertex.bind();
    m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertex.allocate(sg_vertex.constData(), sg_vertex.count()*sizeof(sg_vertex[0]));
    m_vertex.release();


    QOpenGLWidget::update();
    }
    else
        if(mode==1)
        {
                QOpenGLWidget::update();
        }
}


void Qwidgetgl::update()
{

     Input::update();


     if (Input::buttonPressed(Qt::RightButton))
     {
       static const float transSpeed = 0.5f;
       static const float rotSpeed   = 0.5f;


       m_camera.rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
       m_camera.rotate(-rotSpeed * Input::mouseDelta().y(), m_camera.right());


       QVector3D translation;
       if (Input::keyPressed(Qt::Key_W))
       {
         translation += m_camera.forward();
       }
       if (Input::keyPressed(Qt::Key_S))
       {
         translation -= m_camera.forward();
       }
       if (Input::keyPressed(Qt::Key_A))
       {
         translation -= m_camera.right();
       }
       if (Input::keyPressed(Qt::Key_D))
       {
         translation += m_camera.right();
       }
       if (Input::keyPressed(Qt::Key_Q))
       {
         translation -= m_camera.up();
       }
       if (Input::keyPressed(Qt::Key_E))
       {
         translation += m_camera.up();
       }
       m_camera.translate(transSpeed * translation);
     }

    m_transform.rotate(1.0f, QVector3D(0.0f, 0.0f, 0.0f));//


    QOpenGLWidget::update();
}

void Qwidgetgl::right_click(const QPoint &)
{
  // qDebug()<<"right_click_started";
}


void Qwidgetgl::mousePressEvent(QMouseEvent *event)
{
    Input::registerMousePress(event->button());
    //qDebug()<<"button down";
}

void Qwidgetgl::mouseReleaseEvent(QMouseEvent *event)
{
    Input::registerMouseRelease(event->button());
    //qDebug()<<"button up";
}

void Qwidgetgl::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
      event->ignore();
    }
    else
    {
      Input::registerKeyPress(event->key());
    }
    //qDebug()<<"key_press";
}

void Qwidgetgl::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
      event->ignore();
    }
    else
    {
      Input::registerKeyRelease(event->key());
    }
    //qDebug()<<"key_release";
}


