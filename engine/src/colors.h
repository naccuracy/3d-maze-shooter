#ifndef _COLORS_H
#define _COLORS_H

class Color4f{
  public:
  float r, g, b, a;//0..1
  Color4f(float R=0.5, float G=0.5, float B=0.5, float A=1.0);
  Color4f(const Color4f &c);
  ~Color4f();  
};
#endif