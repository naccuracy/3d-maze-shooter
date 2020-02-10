//  ☀️
#include "simple_mesh.h"
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>

SimpleMesh::SimpleMesh() {}

SimpleMesh::~SimpleMesh() {
  points.clear();
  faces.clear();
  name = "";
}

void SimpleMesh::LoadFromCSV(char *fn) {
  FILE *f = NULL;
  char str[256];
  int np = 0, nf = 0, i, nt = 0;
  Point3f p;
  Point2f t;
  float px, py, pz;
  Face3i fc;
  f = fopen(fn, "rt");
  if (f == NULL) {
    printf("\nerror");
    return;
  }
  fscanf(f, "%s", str);
  fscanf(f, "%d;%d;%d", &np, &nf, &nt);
  name = str;
  for (i = 0; i < np; i++) {
    fscanf(f, "%f;%f;%f", &px, &py, &pz);
    p = Point3f(px, py, pz);
    points.push_back(p);
  }
  for (i = 0; i < nf; i++) {
    fscanf(f, "%d;%d;%d\n", &fc.a, &fc.b, &fc.c);
    faces.push_back(fc);
  }
  for (i = 0; i < nt; i++) {
    fscanf(f, "%f;%f", &px, &py);
    t = Point2f(px, py);
    texcoords.push_back(t);
  }
  for (i = 0; i < nf; i++) {
    fscanf(f, "%d;%d;%d\n", &fc.a, &fc.b, &fc.c);
    tex_faces.push_back(fc);
  }
  fclose(f);
  Normalize(sm_flat);
  return;
}

void SimpleMesh::SaveToCSV(char *fn) {
  FILE *f = NULL;
  unsigned int i;
  f = fopen(fn, "wt");
  if (f == NULL)
    return;
  fprintf(f, "%s\n%d;%d;%d\n", name.c_str(), points.size(), faces.size(),
          texcoords.size());
  for (i = 0; i < points.size(); i++) {
    fprintf(f, "%f;%f;%f\n", points[i][0], points[i][1], points[i][2]);
  }
  for (i = 0; i < faces.size(); i++) {
    fprintf(f, "%d;%d;%d\n", faces[i].a, faces[i].b, faces[i].c);
  }
  for (i = 0; i < texcoords.size(); i++) {
    fprintf(f, "%f;%f\n", texcoords[i][0], texcoords[i][1]);
  }
  for (i = 0; i < tex_faces.size(); i++) {
    fprintf(f, "%d;%d;%d\n", tex_faces[i].a, tex_faces[i].b, tex_faces[i].c);
  }
  fclose(f);
  return;
}
//
void SimpleMesh::DrawMesh(Point3f clr) {
  unsigned int i;
  Point3f pa, pb, pc;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  float m[16];
  actor.GetActorMatrix(m);
  glMultMatrixf(m);

  glColor4f(clr[0], clr[1], clr[2], clr[3]); //!!!

  glBegin(GL_LINES);
  for (i = 0; i < faces.size(); i++) {
    pa = points[faces[i].a];
    pb = points[faces[i].b];
    pc = points[faces[i].c];
    glVertex3f(pa[0], pa[1], pa[2]);
    glVertex3f(pb[0], pb[1], pb[2]);
    glVertex3f(pb[0], pb[1], pb[2]);
    glVertex3f(pc[0], pc[1], pc[2]);
    glVertex3f(pc[0], pc[1], pc[2]);
    glVertex3f(pa[0], pa[1], pa[2]);
  }
  glEnd();

  glPopMatrix();
  return;
}
void SimpleMesh::DrawFlatTexturedReferenece() {
  unsigned int nf = faces.size(), i;
  glBegin(GL_TRIANGLES);
  for (i = 0; i < nf; i++) {
    glNormal3fv(normals[i].data);
    glTexCoord2f(texcoords[tex_faces[i].a].data[0],
                 texcoords[tex_faces[i].a].data[1]);
    glVertex4fv(points[faces[i].a].data);
    glTexCoord2f(texcoords[tex_faces[i].b].data[0],
                 texcoords[tex_faces[i].b].data[1]);
    glVertex4fv(points[faces[i].b].data);
    glTexCoord2f(texcoords[tex_faces[i].c].data[0],
                 texcoords[tex_faces[i].c].data[1]);
    glVertex4fv(points[faces[i].c].data);
  }
  glEnd();
}
void SimpleMesh::DrawSmoothTexturedReferenece() {
  unsigned int nf = faces.size(), i;
  glBegin(GL_TRIANGLES);
  for (i = 0; i < nf; i++) {
    glNormal3fv(normals[faces[i].a].data);
    glTexCoord2f(texcoords[tex_faces[i].a].data[0],
                 texcoords[tex_faces[i].a].data[1]);
    glVertex4fv(points[faces[i].a].data);
    glNormal3fv(normals[faces[i].b].data);
    glTexCoord2f(texcoords[tex_faces[i].b].data[0],
                 texcoords[tex_faces[i].b].data[1]);
    glVertex4fv(points[faces[i].b].data);
    glNormal3fv(normals[faces[i].c].data);
    glTexCoord2f(texcoords[tex_faces[i].c].data[0],
                 texcoords[tex_faces[i].c].data[1]);
    glVertex4fv(points[faces[i].c].data);
  }
  glEnd();
}
void SimpleMesh::DrawWithTexCoords(int nrm_type) {
  unsigned int i;
  int a, b, c, ta, tb, tc;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  float m[16];
  actor.GetActorMatrix(m);
  glMultMatrixf(m);
  unsigned int nf = faces.size();
  glBegin(GL_TRIANGLES);
  if (nrm_type == sm_flat)
    for (i = 0; i < nf; i++) {
      a = faces[i].a;
      b = faces[i].b;
      c = faces[i].c;
      ta = tex_faces[i].a;
      tb = tex_faces[i].b;
      tc = tex_faces[i].c;
      glNormal3fv(normals[i].data);
      glTexCoord2f(texcoords[ta].data[0], texcoords[ta].data[1]);
      glVertex4fv(points[a].data);
      glTexCoord2f(texcoords[tb].data[0], texcoords[tb].data[1]);
      glVertex4fv(points[b].data);
      glTexCoord2f(texcoords[tc].data[0], texcoords[tc].data[1]);
      glVertex4fv(points[c].data);
    }
  else
    for (i = 0; i < nf; i++) {
      a = faces[i].a;
      b = faces[i].b;
      c = faces[i].c;
      ta = tex_faces[i].a;
      tb = tex_faces[i].b;
      tc = tex_faces[i].c;
      glNormal3fv(normals[a].data);
      glTexCoord2f(texcoords[ta].data[0], texcoords[ta].data[1]);
      glVertex4fv(points[a].data);
      glNormal3fv(normals[b].data);
      glTexCoord2f(texcoords[tb].data[0], texcoords[tb].data[1]);
      glVertex4fv(points[b].data);
      glNormal3fv(normals[c].data);
      glTexCoord2f(texcoords[tc].data[0], texcoords[tc].data[1]);
      glVertex4fv(points[c].data);
    }
  glEnd();
  glPopMatrix();
  return;
}
void SimpleMesh::DrawSolidReference(Point3f clr,
                                    int nrm_type) // ðèñóåì òðåóãîëüíèêè
{
  unsigned int i;
  glColor4f(clr[0], clr[1], clr[2], clr[3]); //!!!
  unsigned int nf = faces.size();
  glBegin(GL_TRIANGLES);
  if (nrm_type == sm_flat)
    for (i = 0; i < nf; i++) {
      glNormal3fv(normals[i].data);
      glVertex4fv(points[faces[i].a].data);
      glVertex4fv(points[faces[i].b].data);
      glVertex4fv(points[faces[i].c].data);
    }
  else
    for (i = 0; i < nf; i++) {
      glNormal3fv(normals[faces[i].a].data);
      glVertex4fv(points[faces[i].a].data);
      glNormal3fv(normals[faces[i].b].data);
      glVertex4fv(points[faces[i].b].data);
      glNormal3fv(normals[faces[i].c].data);
      glVertex4fv(points[faces[i].c].data);
    }
  glEnd();
  return;
}
void SimpleMesh::DrawSolid(Point3f clr, int nrm_type) // ðèñóåì òðåóãîëüíèêè
{
  unsigned int i;
  int a, b, c;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  float m[16];
  actor.GetActorMatrix(m);
  glMultMatrixf(m);

  glColor4f(clr[0], clr[1], clr[2], clr[3]); //!!!

  unsigned int nf = faces.size();
  glBegin(GL_TRIANGLES);
  if (nrm_type == sm_flat)
    for (i = 0; i < nf; i++) {
      a = faces[i].a;
      b = faces[i].b;
      c = faces[i].c;
      glNormal3fv(normals[i].data);
      glVertex4fv(points[a].data);
      glVertex4fv(points[b].data);
      glVertex4fv(points[c].data);
    }
  else
    for (i = 0; i < nf; i++) {
      a = faces[i].a;
      b = faces[i].b;
      c = faces[i].c;
      glNormal3fv(normals[a].data);
      glVertex4fv(points[a].data);
      glNormal3fv(normals[b].data);
      glVertex4fv(points[b].data);
      glNormal3fv(normals[c].data);
      glVertex4fv(points[c].data);
    }
  glEnd();
  // draw normals
  /*Point3f p;
  glBegin(GL_LINES);
  glColor3f(1.0,1.0,1.0);
  for(i=0;i<nf;i++)
  {
      p=points[faces[i].a] + normals[i];
      glVertex4fv(points[faces[i].a].data);
      glVertex4fv(p.data);
      p=points[faces[i].b] + normals[i];
      glVertex4fv(points[faces[i].b].data);
      glVertex4fv(p.data);
      p=points[faces[i].c] + normals[i];
      glVertex4fv(points[faces[i].c].data);
      glVertex4fv(p.data);
  }
  glEnd();*/
  //
  glPopMatrix();
  return;
}
void SimpleMesh::Normalize(int sm_param) // îáõîä ïðîòèâ ÷àñîâîé ñòðåëêè
{
  Point3f ab, ac, n;
  unsigned int i, nf, np;
  normals_type = sm_param;
  nf = faces.size();
  std::vector<Point3f> norm;
  for (i = 0; i < nf; i++) {
    ab = points[faces[i].b] - points[faces[i].a];
    ac = points[faces[i].c] - points[faces[i].a];
    n = ac.crossProduct(ab);
    n.normalize();
    norm.push_back(n);
  }
  if (sm_param == sm_flat) {
    normals.assign(norm.begin(), norm.end());
    return;
  }
  np = points.size();
  normals.assign(np, Point3f());
  for (i = 0; i < nf; i++) {
    normals[faces[i].a] = normals[faces[i].a] + norm[i];
    normals[faces[i].b] = normals[faces[i].b] + norm[i];
    normals[faces[i].c] = normals[faces[i].c] + norm[i];
  }
  for (i = 0; i < np; i++) {
    normals[i].normalize();
  }
  return;
}
void SimpleMesh::LoadSphere(float R, int seg,
                            int slice) // ñôåðà â íà÷àëå êîîðäèíàò
{
  float a, b, da, db, r, x, y, z;
  da = 180.0 / slice;
  db = 360.0 / seg;
  int i, j;
  for (i = 1; i < slice; i++) {
    a = (270.0 + i * da) * 3.14 / 180.0;
    r = R * cos(a);
    y = R * sin(a);
    for (j = 0; j < seg; j++) {
      b = (90.0 + j * db) * 3.14 / 180.0;
      x = r * cos(b);
      z = r * sin(b);
      points.push_back(Point3f(x, y, z));
    }
  }
  points.push_back(Point3f(0, R, 0));  // âåðõíèé ïîëþñ
  points.push_back(Point3f(0, -R, 0)); // íèæíèé ïîëþñ
  int j1;
  for (i = 0; i < slice - 2; i++) {
    for (j = 0; j < seg; j++) {
      j1 = j + 1;
      if (j1 == seg)
        j1 = 0;
      faces.push_back(
          Face3i(i * seg + j, (i + 1) * seg + j1, (i + 1) * seg + j));
      faces.push_back(Face3i(i * seg + j, i * seg + j1, (i + 1) * seg + j1));
    }
  }
  j1 = points.size() - 1;
  i = points.size() - 2;
  int k = (slice - 2) * seg;
  int k1;
  for (j = 0; j < seg; j++) {
    k1 = j + 1;
    if (k1 == seg)
      k1 = 0;
    faces.push_back(Face3i(i, k + j, k + k1));
    faces.push_back(Face3i(j, j1, k1));
  }
}

void SimpleMesh::SwapFaces(void) {
  int i, n;
  n = faces.size();
  for (i = 0; i < n; i++) {
    faces[i].redirect();
  }
}
void SimpleMesh::CalcBoundingBox() {
  int i, n = points.size();
  min = points[0];
  max = points[0];
  for (i = 1; i < n; i++) {
    if (min[0] > points[i][0])
      min.data[0] = points[i][0];
    if (min[1] > points[i][1])
      min.data[1] = points[i][1];
    if (min[2] > points[i][2])
      min.data[2] = points[i][2];

    if (max[0] < points[i][0])
      max.data[0] = points[i][0];
    if (max[1] < points[i][1])
      max.data[1] = points[i][1];
    if (max[2] < points[i][2])
      max.data[2] = points[i][2];
  }
  return;
}
