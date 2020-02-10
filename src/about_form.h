//  ☀️
#ifndef ABOUT_H_INCLUDED
#define ABOUT_H_INCLUDED

#include "2D/interface.h"

class AboutForm : public Form {
public:
  AboutForm(InterfaceItem *it = NULL, int esc_key_code = 27);
  ~AboutForm();
  int OnMouseDown(int x, int y, int btn); // 1-l 2-r 3-m
  int OnSize(int w, int h);
  void RedrawItem();
  void initInfo();

  Menu *infoMenu;
  std::string ver, vendor, renderer;
};

#endif // ABOUT_H_INCLUDED
