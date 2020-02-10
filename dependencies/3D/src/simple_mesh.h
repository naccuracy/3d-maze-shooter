//  ☀️
#ifndef SIMPLE_MESH_H
#define SIMPLE_MESH_H
#include "face3i.h"
#include "frame.h"
#include "point2f.h"
#include "point3f.h"
#include <string>
#include <vector>
#define sm_flat 0
#define sm_smooth 1

// class Mesh описывает сетку в виде массива координат точек и массива
// треугольных граней
class SimpleMesh {
public:
  SimpleMesh();
  ~SimpleMesh();
  std::vector<Point3f> points;
  std::vector<Point2f> texcoords;
  std::vector<Face3i> tex_faces;
  std::vector<Face3i> faces;
  std::vector<Point3f> normals;
  Point3f min, max; // граничный объем
  int normals_type; // sm_flat sm_smooth
  Frame actor; // класс камеры локальные координаты объекта матрица вида
  std::string name;
  void LoadFromCSV(char *fn);
  void SaveToCSV(char *fn);
  void LoadSphere(float R, int seg, int slice); // сфера в начале координат
  void Normalize(int sm_param);                 // sm_flat sm_smooth
  void SwapFaces(void);
  void DrawMesh(Point3f clr);                // рисует сетку
  void DrawSolid(Point3f clr, int nrm_type); // рисуем треугольники

  void DrawWithTexCoords(int nrm_type);

  void DrawFlatTexturedReferenece();
  void DrawSmoothTexturedReferenece();
  void DrawSolidReference(Point3f clr, int nrm_type);

  void CalcBoundingBox();
};
#endif
