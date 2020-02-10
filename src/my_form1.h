//  ☀️
#ifndef MY_FORM1_H_INCLUDED
#define MY_FORM1_H_INCLUDED

#include "2D/interface.h"

class MyForm1 : public Form {
public:
  MyForm1(InterfaceItem *it = NULL, int esc_key_code = 27);
  virtual ~MyForm1();
  virtual int OnMouseDown(int x, int y, int btn) override; // 1-l 2-r 3-m
  virtual int OnSize(int w, int h) override;
  virtual void RedrawItem() override;
  EditBox *edit1;
  EditBox *edit2;

  ImageLabel *buletsImg;
  ImageLabel *livesImg;
  ImageLabel *deadsImg;
  ImageLabel *keysImg;
  TextLabel *buletsLabel;
  TextLabel *livesLabel;
  TextLabel *deadsLabel;
  TextLabel *keysLabel;
  int aim;

  void updateBuletsCount(int b);
  void updateKeysCount(int b);
  void updateLivesCount(int l);
  void updateDeadsCount(int d);

  int mmvisible;         // ôëàã âèäèìîñòè ìèíèêàðòû
  int mmposx, mmposy;    // ïîçèöèÿ ìèíèêàðòû
  int mmwidth, mmheight; // ðàçìåðû ìèíèêàðòû
  int map_tex_id;
};

#endif // MY_FORM1_H_INCLUDED
