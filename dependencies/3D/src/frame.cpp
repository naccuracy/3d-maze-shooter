//  ☀️
#include "frame.h"
#include "matrix.h"
Frame::Frame(Point3f Position, Point3f Target, Point3f Up) {
  position = Position;
  CalculateUVN(Position, Target, Up);
}

Frame::Frame(const Frame &f) {
  v = f.V();
  n = f.N();
  u = f.U();
  position = f.Position();
}

void Frame::GetCameraMatrix(
    float res[16]) // записывает в переменную res матрицу камеры
{
  Point3f nn = (n * (-1.0));
  res[0] = u[0];
  res[4] = u[1];
  res[8] = u[2];
  res[12] = -(position.dotProduct(u));
  res[1] = v[0];
  res[5] = v[1];
  res[9] = v[2];
  res[13] = -(position.dotProduct(v));
  res[2] = nn[0];
  res[6] = nn[1];
  res[10] = nn[2];
  res[14] = -(position.dotProduct(nn));
  res[3] = 0;
  res[7] = 0;
  res[11] = 0;
  res[15] = 1;
  return;
}

void Frame::GetActorMatrix(
    float res[16]) // записывает в переменную res матрицу актера
{
  res[0] = u[0];
  res[4] = v[0];
  res[8] = n[0];
  res[12] = position[0];
  res[1] = u[1];
  res[5] = v[1];
  res[9] = n[1];
  res[13] = position[1];
  res[2] = u[2];
  res[6] = v[2];
  res[10] = n[2];
  res[14] = position[2];
  res[3] = 0;
  res[7] = 0;
  res[11] = 0;
  res[15] = 1;
  return;
}

void Frame::operator=(const Frame &f) // присваиваниие
{
  v = f.V();
  n = f.N();
  u = f.U();
  position = f.Position();
}

void Frame::set(
    Point3f p, Point3f t,
    Point3f up) // установка позиции цели и вектора направления вверх
{
  position = p;
  CalculateUVN(p, t, up);
}

void Frame::RotateU(float angle) // поворот вокруг вектора U
{
  float m[4][4];
  LoadRotateMatrix44(m, angle, u[0], u[1], u[2]);
  Point3f v1, n1;
  Matrix44_X_Vector4(v.data, m, v1.data);
  v = v1;
  Matrix44_X_Vector4(n.data, m, n1.data);
  n = n1;
  return;
}

void Frame::RotateV(float angle) // поворот вокруг вектора V
{
  float m[4][4];
  LoadRotateMatrix44(m, angle, v[0], v[1], v[2]);
  Point3f u1, n1;
  Matrix44_X_Vector4(u.data, m, u1.data);
  u = u1;
  Matrix44_X_Vector4(n.data, m, n1.data);
  n = n1;
  return;
}

void Frame::RotateN(float angle) // поворот вокруг вектора N
{
  float m[4][4];
  LoadRotateMatrix44(m, angle, n[0], n[1], n[2]);
  Point3f v1, u1;
  Matrix44_X_Vector4(v.data, m, v1.data);
  v = v1;
  Matrix44_X_Vector4(u.data, m, u1.data);
  u = u1;
  return;
}

void Frame::GoU(float step) // перемещениие вдоль вектора U
{
  position = position + (u * step);
  return;
}

void Frame::GoV(float step) // перемещениие вдоль вектора V
{
  position = position + (v * step);
  return;
}
void Frame::GoN(float step) // перемещениие вдоль вектора N
{
  position = position + (n * step);
  return;
}

Point3f Frame::U() const // возвращает вектор U
{
  return u;
}

Point3f Frame::V() const // возвращает вектор V
{
  return v;
}

Point3f Frame::N() const // возвращает вектор N
{
  return n;
}

Point3f Frame::Position() const // возвращает позицию
{
  return position;
}

void Frame::CalculateUVN(Point3f pos, Point3f tag,
                         Point3f up) // расчет системы координат камеры
{
  n = tag - pos;
  n.normalize();
  v = up;
  v.normalize();
  u = n.crossProduct(v);
  u.normalize();
  v = u.crossProduct(n);
  v.normalize();
  return;
}

Frame::~Frame() {}
