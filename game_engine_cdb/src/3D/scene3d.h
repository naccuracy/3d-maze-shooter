#ifndef SCENE3D_H_INCLUDED
#define SCENE3D_H_INCLUDED
#include<vector>
#include<string>
#include"point3f.h"
#include"face3i.h"
#include"frame.h"
#include <GL/gl.h>
#define pos_light 1.0
#define direct_light 0.0

using namespace std;
class SceneItem
{
  public:
  SceneItem();
  ~SceneItem();
  //ссылка на сцену
  string name;
  //actor
  //material reference
  //bounding box
};

class Scene3d
{
  public:
  Scene3d();
  ~Scene3d();
  //список объектов сцены
  vector<SceneItem*> childs;
  //список материалов
  //осещение
  Point3f lmAmbient; //light model ambient
  bool vpType; //view point type local - true; infinity - false;
  bool doubleSide; // double - true; single - false
  int lmColorControl; //  GL_SEPARATE_SPECULAR_COLOR; GL_SINGLE_COLOR
  //камера
  Frame camera;
  //имя сцены
  string name;
  void setLightModel();
};

class nLight
{
    public:
    nLight();
    ~nLight();
    Frame actor;
    string name;
    Point3f pos;//позиция по умолчанию источник позиционный(w = 1, для направленного w =0)
    int id;//GL_LIGHT0 ...
    Point3f ambient;//интенсивность фонового цвета света
    Point3f diffuse;//интенсивность диффузного цвета света
    Point3f specular;//интенсивность цвета зеркального блика
    Point3f spot_dir;//направление света прожектора
    GLfloat spot_exp;//концентрация светового луча
    GLfloat spot_cutoff;//угловая ширина светового луча
    GLfloat const_att;//постоянный фактор ослабления
    GLfloat linear_att;//линейный фактор ослабления
    GLfloat quadr_att;//квадратичный фактор ослабления

    void init();//инициализация
    void on();//включить источник
    void off();//выключить источник
    void setLocationType(float type);//установить тип положения освещения
};
class nMaterial
{
    nMaterial();
    ~nMaterial();
    int side_type;//GL_BACK GL_FRONT GL_FRONT_AND_BACK - на какую сторону граней  накладывать материал
    Point3f ambient;//фоновый цвет материала
    Point3f diffuse;//диффузный цвет материала
    Point3f specular;//зеркальный цвет материала
    float shininess;//показатель зеркального отражения
    Point3f emission;//цвет самосвечения
    Face3i color_indexes;//индексы фонового диффузного и зеркального цветов
    void aplay();//применить материал ко всем следующим полигонам
};
#endif // SCENE3D_H_INCLUDED
