//  ☀️
#ifdef WINDOWS_PLATFORM
#include <windows.h>
#endif
#include "game.h"
#include <SDL2/SDL.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <memory>

std::unique_ptr<Game> g0= nullptr;
// The window we'll be rendering to
SDL_Window *gWindow = NULL;
// OpenGL context
SDL_GLContext gContext;
// Render flag
bool gRenderQuad = true;

// Starts up SDL, creates window, and initializes OpenGL
bool init();
// Initializes matrices and clear color
bool initGL();
// Per frame update
void update();
// Renders quad to the screen
void render();
// Frees media and shuts down SDL
void close();
void reshape(int width, int height);

bool init() {
  // Initialization flag
  bool success = true;
  g0 = std::make_unique<Game>();
  g0->initOptions();
  // winCaption = g0->os.Value("caption");
  int height = g0->intr->height;
  int width = g0->intr->width;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError()
              << std::endl;
    success = false;
  } else {
    // Use OpenGL 2.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    // Create window
    gWindow = SDL_CreateWindow("maze", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width, height,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
      std::cout << "Window could not be created! SDL Error: " << SDL_GetError()
                << std::endl;
      success = false;
    } else {
      // Create context
      gContext = SDL_GL_CreateContext(gWindow);
      if (gContext == NULL) {
        std::cout << "OpenGL context could not be created! SDL Error: "
                  << SDL_GetError() << std::endl;
        success = false;
      } else {
        // Use Vsync
        if (SDL_GL_SetSwapInterval(1) < 0) {
          std::cout << "Warning: Unable to set VSync! SDL Error: "
                    << SDL_GetError() << std::endl;
        }
        // Initialize OpenGL
        if (!initGL()) {
          std::cout << "Unable to initialize OpenGL!" << std::endl;
          success = false;
        }

        // hide mouse cursor and use relative mouse move mode
        if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
          std::cout << "Warning: Unable to set RelativeMouseMode! SDL Error: "
                    << SDL_GetError() << std::endl;
        }
      }
    }
  }
  if (g0 && !g0->inited) {
    g0->init();
    SDL_WarpMouseInWindow(NULL, g0->intr->width / 2, g0->intr->height / 2);
  }
  return success;
}

bool initGL() {
  bool success = true;
  glClearColor(0.5, 0.6, 0.8, 1.0);
  glClearDepth(1.0f);
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glShadeModel(GL_SMOOTH);
  if (g0) {
    glLightfv(GL_LIGHT0, GL_DIFFUSE, g0->white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, g0->white_light);
    reshape(g0->intr->width, g0->intr->height);
  }
  return success;
}

void update() {
  if (g0) {
    g0->OnTimer();
  }
}

void render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (g0) {
    g0->OnPaint();
  }
}

void reshape(int width, int height) {
  glViewport(0, 0, (GLint)width, (GLint)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat AspectRatio = (GLfloat)width / (GLfloat)height;
  gluPerspective(60.0, AspectRatio, 1.0, 2000.0);
  if (g0) {
    g0->OnSize(width, height);
  }
}

void close() {
  // Destroy window
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  if (g0) {
    g0->deinit();
  }
  // Quit SDL subsystems
  SDL_Quit();
}

int main(int argc, char *args[]) {
  // Start up SDL and create window
  if (!init()) {
    printf("Failed to initialize!\n");
  } else {
    // Main loop flag
    bool quit = false;
    // Event handler
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();
    int frames = 0;
    double all_time = 0;
    chrono::duration<double> delta_time;
    auto start_time = chrono::system_clock::now();
    auto end_time = chrono::system_clock::now();
    // While application is running
    while (!quit) {
      end_time = chrono::system_clock::now();
      delta_time = end_time - start_time;
      double dt = delta_time.count();
      // Handle events on queue
      while (SDL_PollEvent(&e) != 0) {
        if (!g0) {
          continue;
        }
        // User requests quit
        if (e.type == SDL_QUIT) {
          quit = true;
        } else if (e.type == SDL_KEYDOWN) {
          g0->OnKeyDown(e.key.keysym.sym);
        } else if (e.type == SDL_KEYUP) {
          g0->OnKeyUp(e.key.keysym.sym); // case SDLK_UP:
        } else if (e.type == SDL_MOUSEMOTION) {
          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);
          g0->OnMouseMove(x, g0->intr->height - y);
          if (!g0->intr->gamePause) {
            SDL_WarpMouseInWindow(NULL, g0->intr->width / 2,
                                  g0->intr->height / 2);
          }
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);
          g0->OnLButtonDown(x, g0->intr->height - y);
        } else if (e.type == SDL_MOUSEBUTTONUP) {
          int x = 0, y = 0;
          SDL_GetMouseState(&x, &y);
          g0->OnLButtonUp(x, g0->intr->height - y);
        } else if (e.type == SDL_WINDOWEVENT_RESIZED ||
                   e.type == SDL_WINDOWEVENT_SIZE_CHANGED) {
          // event->window.windowID
          reshape(e.window.data1, e.window.data2);
        }
      }
      update();
      // Render quad
      render();
      // Update screen
      SDL_GL_SwapWindow(gWindow);
      ++frames;
      all_time += dt;
      start_time = end_time;
      if (g0 && g0->mainloopActive == false) {
        quit = true;
      }
    }
    // Disable text input
    SDL_StopTextInput();
    cout << "FPS: " << frames / all_time << endl;
    cout << "time: " << all_time << " sec" << endl;
    cout << "released resources, exit" << endl;
  }
  // Free resources and close SDL
  close();
  return 0;
}
