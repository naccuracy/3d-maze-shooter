#include "app.h"
#include <iostream>
using namespace std;
App::App()
{
  cout << "App::App" << endl;
  GUIPtr = NULL;
  ScenePtr = NULL;
}
App::~App()
{
  cout << "App::~App" << endl;
}
int App::init(string optionsFile)
{
  cout << "App::init("<< optionsFile <<")" << endl;
}
void App::run()
{
  cout << "App::run" << endl;
}
void App::release()
{
  cout << "App::release" << endl;
}
bool App::mouseDown(const Point2i &p)
{
  cout << "App::mouseDown" << endl;
  if(ScenePtr != nullptr) ScenePtr->mouseDown(p);
  if(GUIPtr != nullptr) GUIPtr->mouseDown(p);
}
void App::mouseUp(const Point2i &p)
{
  cout << "App::mouseUp" << endl;
  if(ScenePtr != nullptr) ScenePtr->mouseUp(p);
  if(GUIPtr != nullptr) GUIPtr->mouseUp(p);
}
bool App::mouseMove(const Point2i &p)
{
  cout << "App::mouseMove" << endl;
  if(ScenePtr != nullptr) ScenePtr->mouseMove(p);
  if(GUIPtr != nullptr) GUIPtr->mouseMove(p);
}
bool App::mouseOver(const Point2i &p)
{
  cout << "App::mouseOver" << endl;
  if(ScenePtr != nullptr) ScenePtr->mouseOver(p);
  if(GUIPtr != nullptr) GUIPtr->mouseOver(p);
}
bool App::keyDown(int symbol)
{
  cout << "App::keyDown" << endl;
  if(ScenePtr != nullptr) ScenePtr->keyDown(symbol);
  if(GUIPtr != nullptr) GUIPtr->keyDown(symbol);
}
bool App::keyUp(int symbol)
{
  cout << "App::keyUp" << endl;
  if(ScenePtr != nullptr) ScenePtr->keyUp(symbol);
  if(GUIPtr != nullptr) GUIPtr->keyUp(symbol);
}
void App::appActivate()
{
  cout << "App::appActivate" << endl;
}
void App::appDeactivate()
{
  cout << "App::appDeactivate" << endl;
}
void App::draw()
{
  cout << "App::draw" << endl;
  if(ScenePtr != nullptr) ScenePtr->draw();
  if(GUIPtr != nullptr) GUIPtr->draw();
}
void App::update(float dt)
{
  cout << "App::update" << endl;
  if(ScenePtr != nullptr) ScenePtr->update(dt);
  if(GUIPtr != nullptr) GUIPtr->update(dt);
}
void App::resize(const Point2i &s)
{
  cout << "App::resize" << endl;
  if(ScenePtr != nullptr) ScenePtr->resize(s);
  if(GUIPtr != nullptr) GUIPtr->resize(s);
}
void App::setGUI(GUI* g)
{
  cout << "App::setGUI" << endl;
  GUIPtr = g;
}
void App::setScene(Scene* s)
{
  cout << "App::setScene" << endl;
  ScenePtr = s;
}
void App::setOption(const std::string& name, const std::string& value)
{
  cout << "App::setOption" << endl;
}
std::string App::getOption(const std::string& name)
{
  cout << "App::getOption" << endl;
}
