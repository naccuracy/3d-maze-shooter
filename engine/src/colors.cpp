#include "colors.h"

Colors4f::Colors4f(float R, float G, float B, float A)
:r(R)
,g(G)
,b(B)
,a(A)
{
}

Colors4f::Color4f(const Color4f &c)
:r(c.r)
,g(c.g)
,b(c.b)
,a(c.a)
{        
}

Colors4f::~Color4f()
{   
} 