//  ☀️
#include "about_form.h"
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>

AboutForm::AboutForm(InterfaceItem *it, int esc_key_code)
    : Form(it, esc_key_code) {
  infoMenu = new Menu(this);
  infoMenu->caption = "infoMenu";
  ver = "";
  vendor = "";
  renderer = "";
  infoMenu->Align(iiAlignCenter, iiAlignCenter);
  infoMenu->Show();
}
void AboutForm::initInfo() {
  std::vector<std::string> sl;
  sl.push_back("DOWN TOGETHER v2.0");
  sl.push_back("");
  sl.push_back("OpenGL Info:");
  sl.push_back("Version: " + ver);
  sl.push_back("Vendor: " + vendor);
  sl.push_back("Renderer: " + renderer);
  sl.push_back("");
  sl.push_back("OSP Studio (c) 2014");
  infoMenu->SetStringItems(sl);
  infoMenu->Align(iiAlignCenter, iiAlignCenter);
}
AboutForm::~AboutForm() { infoMenu = NULL; }

void AboutForm::RedrawItem() {
  Form::RedrawItem();
  return;
}
int AboutForm::OnSize(int w, int h) {
  int res = Form::OnSize(w, h);
  infoMenu->Align(iiAlignCenter, iiAlignCenter);
  return res;
}
int AboutForm::OnMouseDown(int x, int y, int btn) {
  int res = Form::OnMouseDown(x, y, btn);
  return res;
}
