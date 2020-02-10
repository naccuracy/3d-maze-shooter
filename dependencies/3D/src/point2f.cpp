//  ☀️
#include "point2f.h"
#include "n_defs.h"
#include <math.h>
//---------------------------copy constructor-----------------------------
Point2f::Point2f(const Point2f &p) {
  data[0] = p.data[0];
  data[1] = p.data[1];
  data[2] = p.data[2];
}
//---------------------------constructor----------------------------------
Point2f::Point2f(float x, float y) {
  data[0] = x;
  data[1] = y;
  data[2] = 1;
}
//----------------------------destructor----------------------------------
Point2f::~Point2f() {}
// float data[3];//data data[0] = x data[1]= y data[2] = w;
//----------------------------operator=----------------------------------
void Point2f::operator=(const Point2f &p) {
  data[0] = p.data[0];
  data[1] = p.data[1];
  data[2] = p.data[2];
}
//----------------------------operator+----------------------------------
Point2f Point2f::operator+(const Point2f &p) {
  return Point2f(data[0] + p.data[0], data[1] + p.data[1]);
}
//----------------------------operator-----------------------------------
Point2f Point2f::operator-(const Point2f &p) {
  return Point2f(data[0] - p.data[0], data[1] - p.data[1]);
}
//----------------------------operator*----------------------------------
Point2f Point2f::operator*(float a) {
  return Point2f(data[0] * a, data[1] * a);
}
//----------------------------operator==----------------------------------
bool Point2f::operator==(const Point2f &p) {
  return (sqrt((p.data[0] - data[0]) * (p.data[0] - data[0]) +
               (p.data[1] - data[1]) * (p.data[1] - data[1])) < N_ACCURACY_1e3);
}
//----------------------------operator>----------------------------------
bool Point2f::operator>(const Point2f &p) {
  return (
      (data[0] > p.data[0]) ||
      (fabs(data[0] - p.data[0]) < N_ACCURACY_1e3 && (data[1] > p.data[1])));
}
//----------------------------operator<----------------------------------
bool Point2f::operator<(const Point2f &p) {
  return (
      (data[0] < p.data[0]) ||
      (fabs(data[0] - p.data[0]) < N_ACCURACY_1e3 && (data[1] < p.data[1])));
}
//----------------------------operator!=----------------------------------
bool Point2f::operator!=(const Point2f &p) {
  return (sqrt((p.data[0] - data[0]) * (p.data[0] - data[0]) +
               (p.data[1] - data[1]) * (p.data[1] - data[1])) >=
          N_ACCURACY_1e3);
}
//----------------------------operator[]----------------------------------
float Point2f::operator[](int i) { return data[i]; }
//----------------------------return length of vector---------------------
float Point2f::length(void) {
  return sqrt(data[0] * data[0] + data[1] * data[1]);
}
//----------------------------set vector length = 1---------------------
void Point2f::normalize(void) {
  float l = length();
  if (l < N_ACCURACY_1e3)
    return;
  data[0] = data[0] / l;
  data[1] = data[1] / l;
  return;
}
//----------------------------calculate dot product----------------------
float Point2f::dotProduct(const Point2f &p) {
  return data[0] * p.data[0] + data[1] * p.data[1];
}
