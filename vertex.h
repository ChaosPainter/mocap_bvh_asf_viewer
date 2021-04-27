#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>

class Vertex
{
public:

  Vertex(const QVector3D &position, const QVector3D &color);
  static const int PositionTupleSize = 3;
  static const int ColorTupleSize = 3;
  QVector3D m_position;
  QVector3D m_color;

private:

};

#endif // VERTEX_H
