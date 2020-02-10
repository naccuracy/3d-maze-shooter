#ifndef MY_FORM1_H_INCLUDED
#define MY_FORM1_H_INCLUDED

#include"../../src/2D/interface.h"

class MyForm1: public Form
{
    public:
    MyForm1(InterfaceItem *it=NULL);
    ~MyForm1();
    int OnMouseDown(int x, int y, int btn);//1-l 2-r 3-m
    int OnSize(int w, int h);
    void RedrawItem();
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

    int mmvisible;//флаг видимости миникарты
    int mmposx, mmposy;//позиция миникарты
    int mmwidth, mmheight;//размеры миникарты
    int map_tex_id;
};

#endif // MY_FORM1_H_INCLUDED
