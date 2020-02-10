//  ☀️
#include "point3f.h"
#include "n_defs.h"
#include <math.h>
//----------------------------------------------------------------------------
Point3f::Point3f(const Point3f &p) // copy constructor
{
  data[0] = p.data[0];
  data[1] = p.data[1];
  data[2] = p.data[2];
  data[3] = p.data[3];
}
//----------------------------------------------------------------------------
Point3f::Point3f(float x, float y, float z) // constructor
{
  data[0] = x;
  data[1] = y;
  data[2] = z;
  data[3] = 1.0;
}
//----------------------------------------------------------------------------
Point3f::~Point3f() {}
//----------------------------------------------------------------------------
void Point3f::operator=(const Point3f &p) {
  data[0] = p.data[0];
  data[1] = p.data[1];
  data[2] = p.data[2];
  data[3] = p.data[3];
}
//----------------------------------------------------------------------------
Point3f Point3f::operator+(const Point3f &p) {
  return Point3f(data[0] + p.data[0], data[1] + p.data[1], data[2] + p.data[2]);
}
//----------------------------------------------------------------------------
Point3f Point3f::operator-(const Point3f &p) {
  return Point3f(data[0] - p.data[0], data[1] - p.data[1], data[2] - p.data[2]);
}
//----------------------------------------------------------------------------
Point3f Point3f::operator*(float a) {
  return Point3f(data[0] * a, data[1] * a, data[2] * a);
}
//----------------------------------------------------------------------------
bool Point3f::operator==(const Point3f &p) {
  float l = 0.0;
  int i;
  for (i = 0; i < 3; i++) {
    l += (data[i] - p.data[i]) * (data[i] - p.data[i]);
  }
  return sqrt(l) < N_ACCURACY_1e3;
}
//----------------------------------------------------------------------------
bool Point3f::operator>(const Point3f &p) {
  return ((data[0] > p.data[0]) ||
          (data[1] > p.data[1] && fabs(data[0] - p.data[0]) < N_ACCURACY_1e3) ||
          (data[2] > p.data[2] && fabs(data[0] - p.data[0]) < N_ACCURACY_1e3 &&
           fabs(data[1] - p.data[1]) < N_ACCURACY_1e3));
}
//----------------------------------------------------------------------------
bool Point3f::operator<(const Point3f &p) {
  return ((data[0] < p.data[0]) ||
          (data[1] < p.data[1] && fabs(data[0] - p.data[0]) < N_ACCURACY_1e3) ||
          (data[2] < p.data[2] && fabs(data[0] - p.data[0]) < N_ACCURACY_1e3 &&
           fabs(data[1] - p.data[1]) < N_ACCURACY_1e3));
}
//----------------------------------------------------------------------------
bool Point3f::operator!=(const Point3f &p) {
  float l = 0.0;
  int i;
  for (i = 0; i < 3; i++) {
    l += (data[i] - p.data[i]) * (data[i] - p.data[i]);
  }
  return sqrt(l) >= N_ACCURACY_1e3;
}
//----------------------------------------------------------------------------
float Point3f::operator[](int i) { return data[i]; }
//----------------------------------------------------------------------------
float Point3f::length(void) // return length of vector
{
  return sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
}
//----------------------------------------------------------------------------
void Point3f::normalize(void) // set vector length = 1
{
  float l = length();
  if (l <= N_ACCURACY_1e3)
    return;
  data[0] = data[0] / l;
  data[1] = data[1] / l;
  data[2] = data[2] / l;
  return;
}
//----------------------------------------------------------------------------
float Point3f::dotProduct(const Point3f &p) // calculate dot product
{
  return data[0] * p.data[0] + data[1] * p.data[1] + data[2] * p.data[2];
}
//----------------------------------------------------------------------------
Point3f Point3f::crossProduct(const Point3f &p) // cross product
{
  return Point3f((data[1] * p.data[2] - data[2] * p.data[1]),
                 (data[2] * p.data[0] - data[0] * p.data[2]),
                 (data[0] * p.data[1] - data[1] * p.data[0]));
}
