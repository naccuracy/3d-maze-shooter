//  ☀️
#include "face3i.h"

Face3i::Face3i(int A, int B, int C) : a(A), b(B), c(C) {}
Face3i::Face3i(const Face3i &f) {
  a = f.a;
  b = f.b;
  c = f.c;
}

Face3i::~Face3i() {}

void Face3i::operator=(const Face3i &f) {
  a = f.a;
  b = f.b;
  c = f.c;
}

bool Face3i::operator==(const Face3i &f) {
  return ((a == f.a) && (b = f.b) && (c = f.c));
}
bool Face3i::operator!=(const Face3i &f) {
  return ((a != f.a) || (b != f.b) || (c != f.c));
}
bool Face3i::hasVertex(int ind) {
  return ((a == ind) || (b == ind) || (c == ind));
}

bool Face3i::hasEdge(const Edge2i &e) {
  return hasVertex(e.a) && hasVertex(e.b);
}

void Face3i::redirect() {
  int i;
  i = a;
  a = b;
  b = i;
}
