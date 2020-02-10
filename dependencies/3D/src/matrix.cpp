//  ☀️
#include "matrix.h"
#include "n_defs.h"
#include <math.h>

int Matrix44_X_Matrix44(float a[4][4], float b[4][4], float c[4][4]) {
  int i, j, k;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      c[i][j] = 0.0;
      for (k = 0; k < 4; k++) {
        c[i][j] = c[i][j] + a[i][k] * b[k][j];
      }
    }
  }
  return 0;
}

int Vector4_X_Matrix44(float v4[4], float m44[4][4], float rez4[4]) {
  int i, j;
  for (i = 0; i < 4; i++) {
    rez4[i] = 0.0;
    for (j = 0; j < 4; j++) {
      rez4[i] += m44[j][i] * v4[j];
    }
  }
  return 0;
}
int Matrix44_X_Vector4(float v4[4], float m44[4][4], float rez4[4]) {
  int i, j;
  for (i = 0; i < 4; i++) {
    rez4[i] = 0.0;
    for (j = 0; j < 4; j++) {
      rez4[i] += m44[i][j] * v4[j];
    }
  }
  return 0;
}

int LoadIdentityMatrix44(float m44[4][4]) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (i == j)
        m44[i][j] = 1.0;
      else
        m44[i][j] = 0.0;
    }
  }
  return 0;
}

int LoadTranslateMatrix44(float m44[4][4], float dx, float dy, float dz) {
  LoadIdentityMatrix44(m44);
  m44[0][3] = dx;
  m44[1][3] = dy;
  m44[2][3] = dz;
  return 0;
}

int LoadScaleMatrix44(float m44[4][4], float sx, float sy, float sz) {
  LoadIdentityMatrix44(m44);
  m44[0][0] = sx;
  m44[1][1] = sy;
  m44[2][2] = sz;
  return 0;
}

int LoadRotateMatrix44(float m44[4][4], float angle, float x, float y,
                       float z) {
  float a, c, s;
  a = angle * (float)(M_PI / 180.0);
  c = cos(a);
  s = sin(a);
  LoadIdentityMatrix44(m44);
  m44[0][0] = c + (1 - c) * x * x;
  m44[0][1] = (1 - c) * y * x - s * z;
  m44[0][2] = (1 - c) * z * x + s * y;
  m44[1][0] = (1 - c) * x * y + s * z;
  m44[1][1] = c + (1 - c) * y * y;
  m44[1][2] = (1 - c) * z * y - s * x;
  m44[2][0] = (1 - c) * x * z - s * y;
  m44[2][1] = (1 - c) * y * z + s * x;
  m44[2][2] = c + (1 - c) * z * z;
  return 0;
}

int Matrix44ToMatrix16(float m44[4][4], float m16[16]) {
  int i, j, k = 0;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      m16[k] = m44[j][i];
      k++;
    }
  }
  return 0;
}

int Matrix16ToMatrix44(float m16[16], float m44[4][4]) {
  int i, j, k;
  for (i = 0; i < 16; i++) {
    k = i / 4;
    j = i % 4;
    m44[j][k] = m16[i];
  }
  return 0;
}
int TranspoceMatrix44(float m[4][4], float mt[4][4]) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      mt[j][i] = m[i][j];
    }
  }
  return 0;
}
