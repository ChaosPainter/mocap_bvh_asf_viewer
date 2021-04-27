#ifndef TRANSFORM3D_H
#define TRANSFORM3D_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Transform3D
{
public:

    static const QVector3D LocalForward;
    static const QVector3D LocalUp;
    static const QVector3D LocalRight;

  Transform3D();


  void translate(const QVector3D &dt);
  void translate(float dx, float dy, float dz);

  void rotate(const QQuaternion &dr);
  void rotate(float angle, const QVector3D &axis);


private:
  bool m_dirty;
  QVector3D m_translation;
  QVector3D m_scale;
  QQuaternion m_rotation;
  QMatrix4x4 m_world;


};



inline Transform3D::Transform3D() : m_dirty(true), m_scale(1.0f, 1.0f, 1.0f) {}

inline void Transform3D::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void Transform3D::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }


#endif // TRANSFORM3D_H
