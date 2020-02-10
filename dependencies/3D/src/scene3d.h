//  ☀️
#ifndef SCENE3D_H_INCLUDED
#define SCENE3D_H_INCLUDED
#include "face3i.h"
#include "frame.h"
#include "point3f.h"
#include <GL/gl.h>
#include <string>
#include <vector>
#define pos_light 1.0
#define direct_light 0.0

class SceneItem {
public:
  SceneItem();
  ~SceneItem();
  // ññûëêà íà ñöåíó
  std::string name;
  // actor
  // material reference
  // bounding box
};

class Scene3d {
public:
  Scene3d();
  ~Scene3d();
  // ñïèñîê îáúåêòîâ ñöåíû
  std::vector<SceneItem *> childs;
  // ñïèñîê ìàòåðèàëîâ
  // îñåùåíèå
  Point3f lmAmbient;  // light model ambient
  bool vpType;        // view point type local - true; infinity - false;
  bool doubleSide;    // double - true; single - false
  int lmColorControl; //  GL_SEPARATE_SPECULAR_COLOR; GL_SINGLE_COLOR
  // êàìåðà
  Frame camera;
  // èìÿ ñöåíû
  std::string name;
  void setLightModel();
};

class nLight {
public:
  nLight();
  ~nLight();
  Frame actor;
  std::string name;
  Point3f pos; // ïîçèöèÿ ïî óìîë÷àíèþ èñòî÷íèê ïîçèöèîííûé(w = 1, äëÿ
               // íàïðàâëåííîãî w =0)
  int id;          // GL_LIGHT0 ...
  Point3f ambient; // èíòåíñèâíîñòü ôîíîâîãî öâåòà ñâåòà
  Point3f diffuse; // èíòåíñèâíîñòü äèôôóçíîãî öâåòà ñâåòà
  Point3f specular; // èíòåíñèâíîñòü öâåòà çåðêàëüíîãî áëèêà
  Point3f spot_dir;    // íàïðàâëåíèå ñâåòà ïðîæåêòîðà
  GLfloat spot_exp;    // êîíöåíòðàöèÿ ñâåòîâîãî ëó÷à
  GLfloat spot_cutoff; // óãëîâàÿ øèðèíà ñâåòîâîãî ëó÷à
  GLfloat const_att;   // ïîñòîÿííûé ôàêòîð îñëàáëåíèÿ
  GLfloat linear_att;  // ëèíåéíûé ôàêòîð îñëàáëåíèÿ
  GLfloat quadr_att; // êâàäðàòè÷íûé ôàêòîð îñëàáëåíèÿ

  void init(); // èíèöèàëèçàöèÿ
  void on();   // âêëþ÷èòü èñòî÷íèê
  void off();  // âûêëþ÷èòü èñòî÷íèê
  void setLocationType(float type); // óñòàíîâèòü òèï ïîëîæåíèÿ îñâåùåíèÿ
};
class nMaterial {
  nMaterial();
  ~nMaterial();
  int side_type; // GL_BACK GL_FRONT GL_FRONT_AND_BACK - íà êàêóþ ñòîðîíó ãðàíåé
                 // íàêëàäûâàòü ìàòåðèàë
  Point3f ambient;  // ôîíîâûé öâåò ìàòåðèàëà
  Point3f diffuse;  // äèôôóçíûé öâåò ìàòåðèàëà
  Point3f specular; // çåðêàëüíûé öâåò ìàòåðèàëà
  float shininess; // ïîêàçàòåëü çåðêàëüíîãî îòðàæåíèÿ
  Point3f emission;     // öâåò ñàìîñâå÷åíèÿ
  Face3i color_indexes; // èíäåêñû ôîíîâîãî äèôôóçíîãî è çåðêàëüíîãî öâåòîâ
  void aplay(); // ïðèìåíèòü ìàòåðèàë êî âñåì ñëåäóþùèì ïîëèãîíàì
};
#endif // SCENE3D_H_INCLUDED
