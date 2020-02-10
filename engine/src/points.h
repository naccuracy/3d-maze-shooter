#ifndef __POINTS_H
#define __POINTS_H
//TODO add arithmetics
class Point2i
{
public:
    int x,y;
    Point2i();
    Point2i(const Point2i &p);
    Point2i(int _x, int _y);
    ~Point2i();
	void operator = (const Point2i &p);
};

class Point3f
{
public:
    float x,y,z;
    Point3f();
    Point3f(Point3f &p);
    Point3f(float _x, float _y, float _z);
    ~Point3f();
};

class Point4f
{
public:
    float x,y,z,w;
    Point4f();
    Point4f(Point4f &p);
    Point4f(float _x, float _y, float _z, float _w);
    ~Point4f();
};
#endif
