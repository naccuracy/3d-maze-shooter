/* A simple program to show how to set up an X window for OpenGL rendering.
 g++ -I ../../../libs/libpng/ -I ../../../libs/zlib -L ../../../libs/libpng/ -L ../../../libs/zlib/ -pthread -Wall -g main_png.cpp -lpthread -lGL -lGLU -lX11 -lz ../../../libs/libpng/libpng16.a -o 1
 */
#include <iostream>
#include <fstream>
#include <string>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>
#include <GL/glu.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include "png.h"

using namespace std;

static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *dpy;
Window     win;
GLfloat    xAngle = 42.0, yAngle = 82.0, zAngle = 112.0;
GLuint texturePNG;
int tw = 640, th = 480;
string tex_name = "1.png";
void fatalError(const string &message)
{
  cout << "main: " << message << endl;
}

void userReadData(png_structp pngPtr, png_bytep data, png_size_t length) {
    //Here we get our IO pointer back from the read struct.
    //This is the parameter we passed to the png_set_read_fn() function.
    //Our std::istream pointer.
    png_voidp a = png_get_io_ptr(pngPtr);
    //Cast the pointer to std::istream* and read 'length' bytes into 'data'
    ((std::istream*)a)->read((char*)data, length);
}

GLuint png_texture_load(const string &file_name, int * width, int * height)
{
    png_byte header[8];

    ifstream fp(file_name.c_str(), ifstream::binary);//rb?
    if (fp.is_open() == false)
    {
        perror(file_name.c_str());
        return 0;
    }

    // read the header
    fp.read((char*)header, 8);

    if (png_sig_cmp(header, 0, 8))
    {
        cout << "error: " << file_name <<" is not a PNG.\n";
        fp.close();
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        cout <<  "error: png_create_read_struct returned 0.\n";
        fp.close();
        return 0;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        cout << "error: png_create_info_struct returned 0.\n";
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fp.close();
        return 0;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        cout << "error: png_create_info_struct returned 0.\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fp.close();
        return 0;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        cout << "error from libpng\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fp.close();
        return 0;
    }

    // init png reading form istream
    png_set_read_fn(png_ptr,(png_voidp)&fp, userReadData);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);
    string str="";
	switch(color_type){
		case PNG_COLOR_TYPE_GRAY:// (bit depths 1, 2, 4, 8, 16)
		str = "PNG_COLOR_TYPE_GRAY";
		break;
		case PNG_COLOR_TYPE_GRAY_ALPHA:// (bit depths 8, 16)
		str = "PNG_COLOR_TYPE_GRAY_ALPHA";
		break;
		case PNG_COLOR_TYPE_PALETTE:// (bit depths 1, 2, 4, 8)
		str = "PNG_COLOR_TYPE_PALETTE";
		break;
		case PNG_COLOR_TYPE_RGB:// (bit_depths 8, 16)
		str = "PNG_COLOR_TYPE_RGB";
		break;
		case PNG_COLOR_TYPE_RGB_ALPHA:// (bit_depths 8, 16)
		str = "PNG_COLOR_TYPE_RGB_ALPHA";
		break;
		default:
		str = "default";
	};
	cout << "color_type: " << str << endl;
    if (width){ *width = temp_width; }
    if (height){ *height = temp_height; }
    cout << "width: " << *width << endl << "height: " << * height << endl;

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);//N23:WTF
	cout << "rowbytes: " << rowbytes << endl;
    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = new png_byte[rowbytes * temp_height * sizeof(png_byte)];//N23: 15---???
    if (image_data == NULL)
    {
        cout << "error: could not allocate memory for PNG image data\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fp.close();
        return 0;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = new png_bytep[temp_height * sizeof(png_bytep)];
    if (row_pointers == NULL)
    {
        cout << "error: could not allocate memory for PNG row pointers\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete image_data;
        fp.close();
        return 0;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    unsigned int i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);

    // Generate the OpenGL texture object
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp_width, temp_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete image_data;
    delete row_pointers;
    fp.close();
    return texture;
}

void redraw(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluOrtho2D(0, (GLdouble)1024, 0, (GLdouble)768);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslatef(100,100,0);
 glBegin(GL_TRIANGLES);
 glBindTexture(GL_TEXTURE_2D, texturePNG);
 glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
 glTexCoord2f(0.0, 1.0); glVertex2f(0, th);
 glTexCoord2f(1.0, 0.0); glVertex2f(tw, 0);
 
 glTexCoord2f(1.0, 1.0); glVertex2f(tw, th);
 glTexCoord2f(1.0, 0.0); glVertex2f(tw, 0);
 glTexCoord2f(0.0, 1.0); glVertex2f(0, th);
 
 glEnd();
 glXSwapBuffers(dpy, win);/* buffer swap does implicit glFlush */
}

int main(int argc, char **argv)
{
  XVisualInfo         *vi;
  Colormap             cmap;
  XSetWindowAttributes swa;
  GLXContext           glxc;
  XEvent               event;
  int                  dummy;

  /*** (1) open a connection to the X server ***/

  dpy = XOpenDisplay(NULL);
  if (dpy == NULL)
    {fatalError("could not open display");return 0;}
  /*** (2) make sure OpenGL's GLX extension supported ***/
  if(!glXQueryExtension(dpy, &dummy, &dummy))
    {fatalError("X server has no OpenGL GLX extension");return 0;}
  /*** (3) find an appropriate visual ***/
  /* find an OpenGL-capable RGB visual with depth buffer */
  vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
  if (vi == NULL){
    fatalError("no RGB visual with depth buffer");return 0;
  }
  /*** (4) create an OpenGL rendering context  ***/
  /* create an OpenGL rendering context */
  glxc = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                        /* direct rendering if possible */ GL_TRUE);
  if (glxc == NULL)
    {fatalError("could not create rendering context");return 0;}
  /*** (5) create an X window with the selected visual ***/
  /* create an X colormap since probably not using default visual */
  cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
  swa.colormap = cmap;
  swa.border_pixel = 0;
  swa.event_mask = KeyPressMask    | ExposureMask
                 | ButtonPressMask | StructureNotifyMask;
  win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
                      1024, 768, 0, vi->depth, InputOutput, vi->visual,
                      CWBorderPixel | CWColormap | CWEventMask, &swa);
  XSetStandardProperties(dpy, win, "main", "main", None,
                         argv, argc, NULL);
  /*** (6) bind the rendering context to the window ***/
  glXMakeCurrent(dpy, win, glxc);
  /*** (7) request the X window to be displayed on the screen ***/
  XMapWindow(dpy, win);
  /*** (8) configure the OpenGL context for rendering ***/
  glEnable(GL_DEPTH_TEST); /* enable depth buffering */
  glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
  glClearDepth(1.0);       /* pedantic, 1.0 is the default */
  /* frame buffer clears should be to black */
  glClearColor(0.2, 0.2, 0.3, 1.0);
  /* set up projection transform */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
  /* establish initial viewport */
  /* pedantic, full window size is default viewport */
  glViewport(0, 0, 1024, 768);
  texturePNG = png_texture_load(tex_name, &tw, &th);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  fatalError( "Press left mouse button to rotate around X axis" );
  fatalError( "Press middle mouse button to rotate around Y axis" );
  fatalError( "Press right mouse button to rotate around Z axis" );
  fatalError( "Press ESC to quit the application" );
  /*** (9) dispatch X events ***/
  bool running = true;
  while (running)
  {
    do
    {
      XNextEvent(dpy, &event);
      switch (event.type)
      {
        case KeyPress:
        {
          KeySym     keysym;
          XKeyEvent *kevent;
          char       buffer[1];
          /* It is necessary to convert the keycode to a
           * keysym before checking if it is an escape */
          kevent = (XKeyEvent *) &event;
          if (   (XLookupString(kevent,buffer,1,&keysym,NULL) == 1)
              && (keysym == (KeySym)XK_Escape) )
            running = false;
          break;
        }
        case ButtonPress:
          switch (event.xbutton.button)
          {
            case 1: xAngle += 10.0;
              break;
            case 2: yAngle += 10.0;
              break;
            case 3: zAngle += 10.0;
              break;
          }
          break;
        case ConfigureNotify:
          glViewport(0, 0, event.xconfigure.width,
                     event.xconfigure.height);
          /* fall through... */
        case Expose:
          break;
        case DestroyNotify:
          running = false;
          break;
      }
    } while(XPending(dpy)); /* loop to compress events */
    redraw();
  }
  glXMakeCurrent(dpy, win, 0);
  glXDestroyContext(dpy, glxc);
  XDestroyWindow(dpy, win);
  cout << "GOOD!" << endl;
  return 0;
}
