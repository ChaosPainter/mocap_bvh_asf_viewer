#include "camera3d.h"
#include <QDebug>

const QVector3D Camera3D::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera3D::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera3D::LocalRight(1.0f, 0.0f, 0.0f);

Camera3D::Camera3D()
{
    m_dirty=true;
}

void Camera3D::translate(const QVector3D &dt)
{
  m_dirty = true;
  m_translation += dt;
}


void Camera3D::rotate(float angle, const QVector3D &axis)
{
    QQuaternion q = QQuaternion::fromAxisAndAngle(axis, angle);
    m_dirty = true;
    m_rotation = q * m_rotation;
}

const QMatrix4x4 &Camera3D::toMatrix()
{
  if (m_dirty)
  {
    m_dirty = false;
    m_world.setToIdentity();
    m_world.rotate(m_rotation.conjugate());
    m_world.translate(-m_translation);
  }
  return m_world;
}

QVector3D Camera3D::forward() const
{
  return m_rotation.rotatedVector(LocalForward);
}

QVector3D Camera3D::right() const
{
  return m_rotation.rotatedVector(LocalRight);
}

QVector3D Camera3D::up() const
{
  return m_rotation.rotatedVector(LocalUp);
}


