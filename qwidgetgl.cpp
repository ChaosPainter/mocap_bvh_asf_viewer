#include "qwidgetgl.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include "vertex.h"
#include "input.h"


// Front Verticies


#define VERTEX_A Vertex( QVector3D(  0.0f,  1.0f,  0.0f), QVector3D( 1.0f, 0.0f, 0.0f ) )//
#define VERTEX_E Vertex( QVector3D(  0.0f,  1.0f,  0.0f), QVector3D( 0.0f, 1.0f, 0.0f ) )//
#define VERTEX_I Vertex( QVector3D(  0.0f,  1.0f,  0.0f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_B Vertex( QVector3D( -1.0f,  0.0f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )//
#define VERTEX_F Vertex( QVector3D( -1.0f,  0.0f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )//
#define VERTEX_J Vertex( QVector3D( -1.0f,  0.0f,  0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_C Vertex( QVector3D(  1.0f,  0.0f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )//
#define VERTEX_G Vertex( QVector3D(  1.0f,  0.0f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_K Vertex( QVector3D(  1.0f,  0.0f,  0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_D Vertex( QVector3D( 0.0f,  0.0f,  -0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )//
#define VERTEX_H Vertex( QVector3D( 0.0f,  0.0f,  -0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_L Vertex( QVector3D( 0.0f,  0.0f,  -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )

//// Create a colored triangle
//static const Vertex sg_vertexes1[] = {
////Front Face
//    VERTEX_A,VERTEX_B,VERTEX_C,
////Right Face
//    VERTEX_E,VERTEX_D,VERTEX_F,
////Left Face
//    VERTEX_I,VERTEX_G,VERTEX_H,
////Bottom Face
//    VERTEX_J,VERTEX_L,VERTEX_K
//};



#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )

// Back Verticies
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
//// Create a colored cube
//static const Vertex sg_vertexes2[] = {
//  // Face 1 (Front)
//    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
//    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
//  // Face 2 (Back)
//    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
//    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
//  // Face 3 (Top)
//    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
//    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
//  // Face 4 (Bottom)
//    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
//    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
//  // Face 5 (Left)
//    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
//    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
//  // Face 6 (Right)
//    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
//    VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
//};



Qwidgetgl::Qwidgetgl(QWidget *parent): QOpenGLWidget(parent)
{
    sg_vertex= {  // Face 1 (Front)
                  VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
                  VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
                // Face 2 (Back)
                  VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
                  VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
                // Face 3 (Top)
                  VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
                  VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
                // Face 4 (Bottom)
                  VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
                  VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
                // Face 5 (Left)
                  VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
                  VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
                // Face 6 (Right)
                  VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
                  VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
               };

    setFocusPolicy(Qt::FocusPolicy::StrongFocus);
   m_transform.translate(0.0f, 0.0f, 0.0f);//

   QSurfaceFormat format;
   format.setDepthBufferSize(24);
   format.setStencilBufferSize(8);
     format.setVersion(3, 3);
   format.setProfile(QSurfaceFormat::CoreProfile);
   this->setFormat(format);
//   setTextureFormat(format);
}

Qwidgetgl::~Qwidgetgl()
{
    makeCurrent();
    teardownGL();
}

void Qwidgetgl::initializeGL()
{
    qDebug()<<"initializeGl_1\n";
    // Initialize OpenGL Backend
    initializeOpenGLFunctions();
    //connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));//
    printContextInformation();

    // Set global information


    //glDepthMask(GL_TRUE);



    // Application-specific initialization
    {
      // Create Shader (Do not release until VAO is created)
      m_program = new QOpenGLShaderProgram();
      m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.vert");
      m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.frag");
      m_program->link();
      m_program->bind();

      // Cache Uniform Locations
          u_modelToWorld = m_program->uniformLocation("modelToWorld");//
          u_worldToCamera = m_program->uniformLocation("worldToCamera");
          u_cameraToView = m_program->uniformLocation("cameraToView");




      // Create Buffer (Do not release until VAO is created)
      m_vertex.create();
      m_vertex.bind();
      m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
      m_vertex.allocate(sg_vertex.constData(), sg_vertex.count()*sizeof(sg_vertex[0]));

      // Create Vertex Array Object
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
    qDebug()<<"initializeGl_2\n";
}

void Qwidgetgl::resizeGL(int width, int height)
{
    // Currently we are not handling width/height changes
   // (void)width;
    //(void)height;

    m_projection.setToIdentity();
    m_projection.perspective(45.0f, width / float(height), 0.0f, 1000.0f);//
    qDebug()<<"resizeGl_1\n";
}

void Qwidgetgl::paintGL()
{
    glEnable(GL_DEPTH_TEST);
//   glDepthFunc(GL_LEQUAL);
   glEnable(GL_CULL_FACE);


  // glDepthRange(0.0f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   // glClearDepth(1.0f);

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //qDebug()<<"paintGl_1\n";


    // Render using our shader
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
      else if (cube) {
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
    // Currently we have no data to teardown
    // Actually destroy our OpenGL information
    m_object.destroy();
    m_vertex.destroy();
    delete m_program;
}

void Qwidgetgl::refresh(QVector<Vertex> array,int mode)
{


    if(mode==0){

    //glDepthFunc(GL_LEQUAL);


    qDebug()<<"refreshGl_1\n";
    sg_vertex=array;




    // Create Buffer (Do not release until VAO is created)

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
    // Update input
     Input::update();

     // Camera Transformation
     if (Input::buttonPressed(Qt::RightButton))
     {
       static const float transSpeed = 0.5f;
       static const float rotSpeed   = 0.5f;

       // Handle rotations
       m_camera.rotate(-rotSpeed * Input::mouseDelta().x(), Camera3D::LocalUp);
       m_camera.rotate(-rotSpeed * Input::mouseDelta().y(), m_camera.right());

       // Handle translations
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

    // Schedule a redraw
    QOpenGLWidget::update();
}

void Qwidgetgl::right_click(const QPoint &)
{
   qDebug()<<"right_click_started";
}

void Qwidgetgl::printContextInformation()
{
    QString glType;
    QString glVersion;
    QString glProfile;

    // Get Version Information
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // Get Profile Information
  #define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile())
    {
      CASE(NoProfile);
      CASE(CoreProfile);
      CASE(CompatibilityProfile);
    }
  #undef CASE

    // qPrintable() will print our QString w/o quotes around it.
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
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


