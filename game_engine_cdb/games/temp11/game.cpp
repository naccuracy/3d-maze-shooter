#include <iostream>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <X11/X.h>/* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include <stdlib.h>

#include "game.h"

#undef unicode
using namespace std;

static int dblBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
Display *display;
Window win;
GLXContext xglrc;
int winWidth = 100, winHeight = 100;
string winCaption = "0000";
GLboolean needRedraw = GL_FALSE;
GLboolean gameLoop = GL_TRUE;

void fatalError(const string message)
{
 cout << "main: " << message << endl;
 exit(1);
}

//functions
int OnChar(int ch)
{
 return 1;
}
int initOpenGL(void)
{
 GLfloat diffuse[]={0.5, 0.5, 0.5, 1.0};
 GLfloat specular[]={0.7, 0.7, 0.7, 1.0};
 glViewport(0,0,winWidth,winHeight);
	glClearColor(0.5,0.6,0.8,1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
 glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
 glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
 return 0;
}
int OnCreate(void)
{
 cout << "N23: enter to OnCreate" << endl;
 XVisualInfo *vi;
 Colormap cmap;
 XSetWindowAttributes swa;
 int dummy;
 display = XOpenDisplay(NULL);
 if (display == NULL)
  fatalError("could not open display");
 if(!glXQueryExtension(display, &dummy, &dummy))
  fatalError("X server has no OpenGL GLX extension");
 /* find an OpenGL-capable RGB visual with depth buffer */
 vi = glXChooseVisual(display, DefaultScreen(display), dblBuf);
 if (vi == NULL)
 {
  fatalError("no RGB visual with depth buffer");
 }
 /* create an OpenGL rendering context */
 xglrc = glXCreateContext(display, vi, /* no shared dlists */ None,
 /* direct rendering if possible */ GL_TRUE);
 if (xglrc == NULL)
  fatalError("could not create rendering context");
 winHeight = 600;
 winWidth = 800;
 /* create an X colormap since probably not using default visual */
 cmap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
 swa.colormap = cmap;
 swa.border_pixel = 0;
 swa.event_mask = KeyPressMask | ExposureMask | ButtonPressMask | StructureNotifyMask;
 win = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0,
 winWidth, winHeight, 0, vi->depth, InputOutput, vi->visual,
 CWBorderPixel | CWColormap | CWEventMask, &swa);
 XSetStandardProperties(display, win, "main", "main", None, NULL, 0, NULL);
 glXMakeCurrent(display, win, xglrc);
 XMapWindow(display, win);
 initOpenGL();
 needRedraw = GL_TRUE;
 gameLoop = GL_TRUE;
 return 0;
}
int OnDestroy()
{
 glXMakeCurrent(display, win, NULL);
 glXDestroyContext(display,xglrc);
 return 0;
}
int OnKeyDown(int key)
{
 return 1;
}
int OnKeyUp(int key)
{
 return 1;
}
int OnLButtonDown(short x, short y)
{
 return 1;
}
int OnLButtonUp(short x, short y)
{
 return 1;
}
int OnMouseMove(short x, short y)
{
 return 1;
}
int OnPaint()
{
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslatef(0.0, 0.0, -20.0);
 glBegin(GL_TRIANGLES);
 glColor3f(0.5,0.6,0.5);
 glVertex3f(10,0,0);glVertex3f(0,10,0);glVertex3f(0,0,0);
 glColor3f(1.0,0.6,0.0);
 glVertex3f(0,0,2);glVertex3f(-14,0,2);glVertex3f(0,-14,2);
 glEnd();
 glXSwapBuffers(display,win);
 needRedraw = GL_FALSE;
 return 0;
}
 int OnSize(int wi, int he)
{
 glViewport(0,0,wi,he);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 GLfloat AspectRatio = (GLfloat)wi/(GLfloat)he;
 gluPerspective(60.0,AspectRatio,1.0,2000.0);

 needRedraw = GL_TRUE;
 return 0;
}
int OnTimer()
{
 needRedraw = true;
 return 0;
}
int main (int argc, char**argv)
{
 cout << "N23: app is started!" << endl;
 XEvent event;
 OnCreate();
 cout << "N23:175" << endl;
 while (gameLoop)
 {
  do
  {
   XNextEvent(display, &event);
   switch (event.type)
   {
    case KeyPress:
    {
     KeySym keysym;
     XKeyEvent *kevent;
     char buffer[1];
     /* It is necessary to convert the keycode to a
      * keysym before checking if it is an escape */
     kevent = (XKeyEvent *) &event;
     if ( (XLookupString((XKeyEvent *)&event,buffer,1,&keysym,NULL) == 1)
           && (keysym == (KeySym)XK_Escape) )
     {
      gameLoop = GL_FALSE;
     }
     break;
    }
    case ButtonPress:
     cout << "\n" << event.xbutton.button;
     break;
    case ConfigureNotify:
     OnSize(event.xconfigure.width, event.xconfigure.height);
    /* fall through... */
    case Expose:
     needRedraw = GL_TRUE;
     break;
   }
  } while(XPending(display)); /* loop to compress events */
  if (needRedraw)
  {
   OnPaint();
   cout << "N23:213" << endl;
  }
 }
 OnDestroy();
 return 0;
}
