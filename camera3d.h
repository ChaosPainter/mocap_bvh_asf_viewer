#ifndef CAMERA_H
#define CAMERA_H

#include "transform3d.h"

class Camera3D
{
public:


  static const QVector3D LocalForward;
  static const QVector3D LocalUp;
  static const QVector3D LocalRight;


  Camera3D();

  void translate(const QVector3D &dt);
  void rotate(float angle, const QVector3D &axis);

  const QMatrix4x4& toMatrix();

  // Queries
  QVector3D forward() const;
  QVector3D right() const;
  QVector3D up() const;

private:
  bool m_dirty;
  QVector3D m_translation;
  QQuaternion m_rotation;
  QMatrix4x4 m_world;


};

Q_DECLARE_TYPEINFO(Camera3D, Q_MOVABLE_TYPE);


#endif // CAMERA_H
