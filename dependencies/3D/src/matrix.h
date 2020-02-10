//  ☀️
#ifndef MATRIX_H
#define MATRIX_H
// поддерживается пока только трехмерные вектора и матрицы преобразования
// #include"point2f.h"
#include "point3f.h"
// функции для работы с матрицами
int TranspoceMatrix44(float m[4][4], float mt[4][4]); // транспонирование
                                                      // матрицы
int Matrix16ToMatrix44(
    float m16[16], float m44[4][4]); // переводим OPenGL матрицу в классическую
int Matrix44ToMatrix16(
    float m44[4][4], float m16[16]); // переводим классическую в OPenGL матрицу
int LoadRotateMatrix44(float m44[4][4], float angle, float x, float y,
                       float z); // создать матрицу вращения
int LoadScaleMatrix44(float m44[4][4], float sx, float sy,
                      float sz); // создать матрицу масштабирования
int LoadTranslateMatrix44(float m44[4][4], float dx, float dy,
                          float dz); // создать матрицу перемещения
int LoadIdentityMatrix44(float m44[4][4]); // создать единичную матрицу
int Matrix44_X_Vector4(float v4[4], float m44[4][4], float rez4[4]);  // m44*v4
int Vector4_X_Matrix44(float v4[4], float m44[4][4], float rez4[4]);  // v4*m44
int Matrix44_X_Matrix44(float a[4][4], float b[4][4], float c[4][4]); // m44*m44
#endif
