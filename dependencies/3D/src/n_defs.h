//  ☀️
#ifndef N_DEFS_H
#define N_DEFS_H
#define _USE_MATH_DEFINES
//
#define N_ACCURACY_1e3 1e-3
#define N_ACCURACY_1e6 1e-6
#define N_ACCURACY_1e9 1e-9
// микросы выбора минимального максимального значений
#define MIN3(A, B, C)                                                          \
  ((A) < (B) ? ((A) < (C) ? (A) : (C)) : ((B) < (C) ? (B) : (C)))
#define MAX3(A, B, C)                                                          \
  ((A) > (B) ? ((A) > (C) ? (A) : (C)) : ((B) > (C) ? (B) : (C)))
#define MIN2(A, B) ((A) < (B) ? (A) : (B))
#define MAX2(A, B) ((A) > (B) ? (A) : (B))
#endif
