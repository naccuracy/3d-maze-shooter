//  ☀️
#ifndef POINT3F_H
#define POINT3F_H
class Point3f {
public:
  Point3f(const Point3f &p);                            // copy constructor
  Point3f(float x = 0.0, float y = 0.0, float z = 0.0); // constructor
  ~Point3f();

  float data[4]; // data data[0] = x data[1]= y data[2] = z data[3] = w;

  void operator=(const Point3f &p);
  Point3f operator+(const Point3f &p);
  Point3f operator-(const Point3f &p);
  Point3f operator*(float a);
  bool operator==(const Point3f &p);
  bool operator>(const Point3f &p);
  bool operator<(const Point3f &p);
  bool operator!=(const Point3f &p);
  float operator[](int i);

  float length(void);                     // return length of vector
  void normalize(void);                   // set vector length = 1
  float dotProduct(const Point3f &p);     // calculate dot product
  Point3f crossProduct(const Point3f &p); // cross product
};
#endif
