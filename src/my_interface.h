//  ☀️
#ifndef MY_INTERFACE_H_INCLUDED
#define MY_INTERFACE_H_INCLUDED
#include "2D/interface.h"
#include "about_form.h"
#include "my_form1.h"
#include "opt_form.h"

class MyInterface : public Interface {
public:
  MyInterface(int w = 640, int h = 480, vectorFont *f = NULL);
  virtual ~MyInterface();
  void Init(OptionsSet *ptr);

  OptForm *f1;              // îïöèè
  MyForm1 *f2;              // ãëàâíàÿ ôîðìà
  AboutForm *f3;            // ôîðìà ñ èíôîðìàöèåé
  Menu *mainMenu;           // ãëàâíîå ìåíþ
  ProgressBar *loadingBar;  // ñòàòóñ çàãðóçêè ïðîãðåññ
  TextLabel *loadingStatus; // ñòàòóñ çàãðóçêè òåêñò
  virtual int OnMouseDown(int x, int y, int btn) override;
  virtual int OnKeyDown(int c) override;
  virtual int OnMouseMove(int x, int y) override;
  virtual void RedrawItem() override;
  int setLoadingProgress(int p, string msg);

  int quitFlag;
  int gamePause;
  int newGame;
  int mousex = 0;
  int mousey = 0;
};
#endif // MY_INTERFACE_H_INCLUDED
