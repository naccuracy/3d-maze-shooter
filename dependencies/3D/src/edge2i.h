//  ☀️
#ifndef EDGE2I_H
#define EDGE2I_H
// класс описывающий ребро(отрезок прямой) хранящее индексы точек в списке точек
// объекта
class Edge2i {
public:
  int a, b;
  Edge2i(int a = -1, int b = -1);
  Edge2i(const Edge2i &e);
  void operator=(const Edge2i &e);
  bool operator==(const Edge2i &e);
  bool operator!=(const Edge2i &e);
  bool hasIndex(int i);
  void flip();
};
#endif
