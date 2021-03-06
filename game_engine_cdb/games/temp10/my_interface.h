#ifndef MY_INTERFACE_H_INCLUDED
#define MY_INTERFACE_H_INCLUDED
#include"../../src/2D/interface.h"
#include "my_form1.h"
#include "opt_form.h"
#include"about_form.h"

class MyInterface: public Interface
{
  public:
  MyInterface(int w=640, int h=480, vectorFont* f=NULL);
  virtual ~MyInterface();
  void Init(OptionsSet *ptr);

  OptForm *f1;//�����
  MyForm1 *f2;//������� �����
  AboutForm *f3;//����� � �����������
  Menu *mainMenu;//������� ����
  ProgressBar *loadingBar;//������ �������� ��������
  TextLabel* loadingStatus;//������ �������� �����
  int OnMouseDown(int x, int y, int btn);
  int OnKeyDown(int c);
  int setLoadingProgress(int p, string msg);

  int quitFlag;
  int gamePause;
  int newGame;
};
#endif // MY_INTERFACE_H_INCLUDED
