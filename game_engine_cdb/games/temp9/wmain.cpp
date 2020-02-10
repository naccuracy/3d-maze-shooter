#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "game.h"

#undef unicode
using namespace std;

char szClassName[128] = "MyOpenGLGameWindow";
HGLRC hglrc;
HDC hdc;
int winWidth = 100, winHeight = 100;
string winCaption = "0000";
Game *g0 = NULL;

//functions
int OnChar(int ch)
{
    return g0->OnChar(ch);;
}
int initOpenGL(HWND wnd)
{
    static PIXELFORMATDESCRIPTOR pfd=
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,
        0,0,0,0,0,0,
        0,0,
        0,0,0,0,0,
        16,
        0,0,0,0,0,0,0
    };
    hdc = GetDC(wnd);
    int nPixelFormat = ChoosePixelFormat(hdc,&pfd);
    SetPixelFormat(hdc,nPixelFormat,&pfd);
    hglrc = wglCreateContext(hdc);
    wglMakeCurrent(hdc,hglrc);
	glViewport(0,0,winWidth,winHeight);
	glClearColor(0.5,0.6,0.8,1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,g0->white_light);
    glLightfv(GL_LIGHT0,GL_SPECULAR,g0->white_light);
    return 0;
}
int OnCreate(HWND hwnd)
{
    initOpenGL(hwnd);
    SetTimer(hwnd,33,25,NULL);//40Hz
    InvalidateRect(hwnd,NULL,true);
    if(!g0->inited)g0->init();
    return 0;
}
int OnDestroy(HWND hwnd)
{
	KillTimer(hwnd,101);
    wglMakeCurrent(hdc,NULL);
    wglDeleteContext(hglrc);
    ReleaseDC(hwnd,hdc);
    PostQuitMessage (0);
    return 0;
}
int OnKeyDown(int key)
{
    return g0->OnKeyDown(key);
}
int OnKeyUp(int key)
{
    return g0->OnKeyUp(key);
}
int OnLButtonDown(short x, short y)
{
    return g0->OnLButtonDown(x,winHeight-y);
}
int OnLButtonUp(short x, short y)
{
    return g0->OnLButtonUp(x,winHeight-y);
}
int OnMouseMove(short x, short y)
{
    return g0->OnMouseMove(x,winHeight-y);
}
int OnPaint(HWND hwnd)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    g0->OnPaint();
    SwapBuffers(hdc);
    ValidateRect(hwnd,NULL);
    return 0;
}
int OnSize(HWND hwnd, int wi, int he)
{
    glViewport(0,0,wi,he);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat AspectRatio = (GLfloat)wi/(GLfloat)he;
    gluPerspective(60.0,AspectRatio,1.0,2000.0);
    g0->OnSize(wi, he);
	InvalidateRect(hwnd,NULL,true);
    return 0;
}
int OnTimer(HWND hwnd)
{
    g0->OnTimer();
    InvalidateRect(hwnd,NULL,true);
    return 0;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_LBUTTONDOWN:OnLButtonDown(LOWORD(lParam),HIWORD(lParam));break;
        case WM_LBUTTONUP:OnLButtonUp(LOWORD(lParam),HIWORD(lParam));break;
        case WM_MOUSEMOVE:OnMouseMove(LOWORD(lParam),HIWORD(lParam));break;
        case WM_CREATE:OnCreate(hwnd);break;
        case WM_DESTROY:OnDestroy(hwnd);break;
        case WM_SIZE:OnSize(hwnd, LOWORD(lParam),HIWORD(lParam));break;
        case WM_KEYDOWN:OnKeyDown((int)wParam);break;
        case WM_KEYUP:OnKeyUp((int)wParam);break;
        case WM_CHAR:OnChar((unsigned char)wParam);break;
        case WM_TIMER:OnTimer(hwnd);break;
        case WM_PAINT:OnPaint(hwnd);break;
        default: return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{

    HWND hWnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC ;/* OpenGL window style */
    wincl.cbSize = sizeof (WNDCLASSEX);
    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, 0);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
	{
        MessageBox(NULL,"RegisterClassEx","ERROR",0);
		return 0;
	}
	g0 = new Game();
	g0->initOptions();
	winCaption = g0->os.Value("caption");
	winHeight = g0->intr->height;
	winWidth = g0->intr->width;
    /* The class is registered, let's create the program*/
	hWnd = NULL;
    hWnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           winCaption.c_str(),       /* Title Text */
           WS_POPUPWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           winWidth,                 /* The programs width */
           winHeight,                 /* and height in pixels */
           NULL,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
	if(hWnd == NULL)
	{
		MessageBox(NULL,"CreateWindowEx","ERROR",0);
		return 0;
	}
    /* Make the window visible on the screen */
    ShowWindow (hWnd, nCmdShow);
	/* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
		if(g0->mainloopActive == false)
		{
			DestroyWindow(hWnd);
		}
    }
	g0->deinit();
	delete g0;
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
