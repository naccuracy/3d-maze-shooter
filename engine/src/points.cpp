#include "points.h"

Point2i::Point2i()
:x(0)
,y(0)
{
}

Point2i::Point2i(const Point2i &p)
:x(p.x)
,y(p.y)
{
}

void Point2i::operator = (const Point2i &p)
{
	x = p.x;
	y = p.y;
}

Point2i::Point2i(int _x, int _y)
:x(_x)
,y(_y)
{
}

Point2i::~Point2i()
{
}

Point3f::Point3f()
:x(0.f)
,y(0.f)
,z(0.f)
{
}

Point3f::Point3f(Point3f &p)
:x(p.x)
,y(p.y)
,z(p.z)
{
}

Point3f::Point3f(float _x, float _y, float _z)
:x(_x)
,y(_y)
,z(_z)
{
}

Point3f::~Point3f()
{
}

Point4f::Point4f()
:x(0.f)
,y(0.f)
,z(0.f)
,w(1.f)
{
}

Point4f::Point4f(Point4f &p)
:x(p.x)
,y(p.y)
,z(p.z)
,w(p.w)
{
}

Point4f::Point4f(float _x, float _y, float _z, float _w)
:x(_x)
,y(_y)
,z(_z)
,w(_w)
{
}

Point4f::~Point4f()
{
}