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
  //������ �� �����
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
  //������ �������� �����
  vector<SceneItem*> childs;
  //������ ����������
  //��������
  Point3f lmAmbient; //light model ambient
  bool vpType; //view point type local - true; infinity - false;
  bool doubleSide; // double - true; single - false
  int lmColorControl; //  GL_SEPARATE_SPECULAR_COLOR; GL_SINGLE_COLOR
  //������
  Frame camera;
  //��� �����
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
    Point3f pos;//������� �� ��������� �������� �����������(w = 1, ��� ������������� w =0)
    int id;//GL_LIGHT0 ...
    Point3f ambient;//������������� �������� ����� �����
    Point3f diffuse;//������������� ���������� ����� �����
    Point3f specular;//������������� ����� ����������� �����
    Point3f spot_dir;//����������� ����� ����������
    GLfloat spot_exp;//������������ ��������� ����
    GLfloat spot_cutoff;//������� ������ ��������� ����
    GLfloat const_att;//���������� ������ ����������
    GLfloat linear_att;//�������� ������ ����������
    GLfloat quadr_att;//������������ ������ ����������

    void init();//�������������
    void on();//�������� ��������
    void off();//��������� ��������
    void setLocationType(float type);//���������� ��� ��������� ���������
};
class nMaterial
{
    nMaterial();
    ~nMaterial();
    int side_type;//GL_BACK GL_FRONT GL_FRONT_AND_BACK - �� ����� ������� ������  ����������� ��������
    Point3f ambient;//������� ���� ���������
    Point3f diffuse;//��������� ���� ���������
    Point3f specular;//���������� ���� ���������
    float shininess;//���������� ����������� ���������
    Point3f emission;//���� ������������
    Face3i color_indexes;//������� �������� ���������� � ����������� ������
    void aplay();//��������� �������� �� ���� ��������� ���������
};
#endif // SCENE3D_H_INCLUDED
