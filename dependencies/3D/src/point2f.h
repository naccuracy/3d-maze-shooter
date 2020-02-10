//  ☀️
#ifndef POINT2F_H
#define POINT2F_H
class Point2f {
public:
  Point2f(const Point2f &p);             // copy constructor
  Point2f(float x = 0.0, float y = 0.0); // constructor
  ~Point2f();

  float data[3]; // data data[0] = x data[1]= y data[2] = w;

  void operator=(const Point2f &p);
  Point2f operator+(const Point2f &p);
  Point2f operator-(const Point2f &p);
  Point2f operator*(float a);
  bool operator==(const Point2f &p);
  bool operator>(const Point2f &p);
  bool operator<(const Point2f &p);
  bool operator!=(const Point2f &p);
  float operator[](int i);

  float length(void);                 // return length of vector
  void normalize(void);               // set vector length = 1
  float dotProduct(const Point2f &p); // calculate dot product
};
#endif
