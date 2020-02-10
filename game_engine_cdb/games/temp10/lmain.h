#ifndef LMAIN_H_INCLUDED
#define LMAIN_H_INCLUDED

int OnChar(int ch);
int OnCreate(void);
int OnDestroy(void);
int OnKeyDown(int key);//key down event handler
int OnKeyUp(int key);//key up event handler
int OnLButtonDown(short x, short y);//left mouse button down event handler
int OnLButtonUp(short x, short y);//left mouse button up event handler
int OnMouseMove(short x, short y);//mouse move event handler
int OnPaint(void);//repaint
int OnSize(void, int wi, int he);//resize handle
int OnTimer(void);
int main (int argc, char**argv);
int initOpenGL(void);

#endif // LMAIN_H_INCLUDED
