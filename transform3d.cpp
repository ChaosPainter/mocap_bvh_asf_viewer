#include "transform3d.h"
#include <QDebug>

const QVector3D Transform3D::LocalForward(0.0f, 0.0f, 1.0f);
const QVector3D Transform3D::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Transform3D::LocalRight(1.0f, 0.0f, 0.0f);

void Transform3D::translate(const QVector3D &dt)
{
  m_dirty = true;
  m_translation += dt;
}

void Transform3D::rotate(const QQuaternion &dr)
{
  m_dirty = true;
  m_rotation = dr * m_rotation;
}




