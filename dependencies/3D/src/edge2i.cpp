//  ☀️
#include "edge2i.h"

Edge2i::Edge2i(int _a, int _b) : a(_a), b(_b){};
Edge2i::Edge2i(const Edge2i &e) {
  a = e.a;
  b = e.b;
}

void Edge2i::operator=(const Edge2i &e) {
  a = e.a;
  b = e.b;
}

bool Edge2i::operator==(const Edge2i &e) { return (a == e.a && b == e.b); }

bool Edge2i::operator!=(const Edge2i &e) { return (a != e.a || b != e.b); }

bool Edge2i::hasIndex(int i) { return (i == a || i == b); }

void Edge2i::flip() {
  int i;
  i = a;
  a = b;
  b = i;
}
