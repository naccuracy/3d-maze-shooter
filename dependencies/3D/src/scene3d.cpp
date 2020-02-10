//  ☀️
#include "scene3d.h"
#include <GL/gl.h>
#include <GL/glu.h>

Scene3d::Scene3d() {
  lmAmbient = Point3f(0.2, 0.2, 0.2);
  lmAmbient.data[3] = 1.0;
  vpType = false; // áåñêîíå÷íî óäàëåííàÿ òî÷êà íàáëþäåíèÿ
  doubleSide = false; // îñâåùàåòñÿ òîëüêî ëèöåâàÿ ñòîðîíà
  // lmColorControl = GL_SINGLE_COLOR; //ïî óìîë÷àíèþ
  lmColorControl = 0x81F9;
}

Scene3d::~Scene3d() {}

void Scene3d::setLightModel() {
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmAmbient.data);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, vpType);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, doubleSide);
  // glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, lmColorControl);
  glLightModeli(0x81F8, lmColorControl);
  return;
}

// èñòî÷íèê ñâåòà
nLight::nLight() {
  name = "nLight0";
  pos = Point3f(0.0, 0.0, 1.0);
  setLocationType(direct_light);
  id = GL_LIGHT0;
  ambient = Point3f(0.0, 0.0, 0.0);
  ambient.data[3] = 1.0;
  diffuse = Point3f(1.0, 1.0, 1.0);
  diffuse.data[3] = 1.0;
  specular = diffuse;
  spot_dir = Point3f(0.0, 0.0, -1.0);
  spot_exp = 0.0;
  spot_cutoff = 180.0;
  const_att = 1.0;
  linear_att = 0.0;
  quadr_att = 0.0;
}
nLight::~nLight() {}
void nLight::setLocationType(float type) { pos.data[3] = type; }
// íàñòðîéêà openGL
void nLight::init() {
  glLightfv(id, GL_AMBIENT, ambient.data);
  glLightfv(id, GL_DIFFUSE, diffuse.data);
  glLightfv(id, GL_SPECULAR, specular.data);
  glLightfv(id, GL_POSITION, pos.data);
  glLightf(id, GL_CONSTANT_ATTENUATION, const_att);
  glLightf(id, GL_LINEAR_ATTENUATION, linear_att);
  glLightf(id, GL_QUADRATIC_ATTENUATION, quadr_att);
  if (spot_cutoff <= 90.0 && spot_cutoff > 0.0) {
    glLightf(id, GL_SPOT_CUTOFF, spot_cutoff);
    glLightfv(id, GL_SPOT_DIRECTION, spot_dir.data);
    glLightf(id, GL_SPOT_EXPONENT, spot_exp);
  }
  return;
}
// âêëþ÷åíèå
void nLight::on() { glEnable(id); }
// âûêëþ÷åíèå
void nLight::off() { glDisable(id); }
//================ìàòåðèàëû
nMaterial::nMaterial() {
  // çíà÷åíèÿ ïî óìîë÷àíèþ
  side_type = GL_FRONT;
  ambient = Point3f(0.2, 0.2, 0.2);
  diffuse = Point3f(0.8, 0.8, 0.8);
  specular = Point3f(0.0, 0.0, 0.0);
  shininess = 0.0;
  emission = Point3f(0.0, 0.0, 0.0);
  Face3i color_indexes = Face3i(0, 1, 1);
}
nMaterial::~nMaterial() {}
void nMaterial::aplay() {
  glMaterialfv(side_type, GL_AMBIENT, ambient.data);
  glMaterialfv(side_type, GL_DIFFUSE, diffuse.data);
  glMaterialfv(side_type, GL_SPECULAR, specular.data);
  glMaterialfv(side_type, GL_EMISSION, emission.data);
  glMaterialf(side_type, GL_SHININESS, shininess);
  int ar[3];
  ar[0] = color_indexes.a;
  ar[1] = color_indexes.b;
  ar[2] = color_indexes.c;
  glMaterialiv(side_type, GL_COLOR_INDEXES, ar);
  return;
}
