#ifndef WMAIN_H_INCLUDED
#define WMAIN_H_INCLUDED

int OnChar(int ch);
int OnCreate(HWND hwnd);
int OnDestroy(HWND hwnd);
int OnKeyDown(int key);//key down event handler
int OnKeyUp(int key);//key up event handler
int OnLButtonDown(short x, short y);//left mouse button down event handler
int OnLButtonUp(short x, short y);//left mouse button up event handler
int OnMouseMove(short x, short y);//mouse move event handler
int OnPaint(HWND hwnd);//repaint
int OnSize(HWND hwnd, int wi, int he);//resize handle
int OnTimer(HWND hwnd);
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow);
int initOpenGL(HWND wnd);

#endif // WMAIN_H_INCLUDED
