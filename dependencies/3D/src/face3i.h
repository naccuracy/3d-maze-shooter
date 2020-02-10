//  ☀️
#ifndef FACE3I_H
#define FACE3I_H
#include "edge2i.h"

class Face3i {
public:
  int a, b, c;
  Face3i(int A = 0, int B = 0, int C = 0);
  Face3i(const Face3i &f);
  ~Face3i();
  void operator=(const Face3i &f);
  bool operator==(const Face3i &f);
  bool operator!=(const Face3i &f);
  bool hasVertex(int ind);
  bool hasEdge(const Edge2i &e);
  void redirect(); // изменение порядка обхода вершин
};
#endif
