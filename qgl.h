#ifndef QGL_H
#define QGL_H
#include <QGLWidget>
#include <QWidget>

class QGL : public QGLWidget
{Q_OBJECT
public:
     explicit QGL(QWidget *parent = 0);
    ~QGL();

signals:


public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void draw();

    int xRot;
    int yRot;
    int zRot;

    QPoint lastPos;
};

#endif // QGL_H
