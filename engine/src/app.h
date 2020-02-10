#ifndef __APP_H
#define __APP_H
#include "points.h"
#include "scene.h"
#include "gui.h"

#include <map>
#include <string>

class App
{
 public:
	App();
	~App();
	int init(string optionsFile);
	void run();
	void release();
	bool mouseDown(const Point2i &p);
	void mouseUp(const Point2i &p);
	bool mouseMove(const Point2i &p);
	bool mouseOver(const Point2i &p);
	bool keyDown(int symbol);
	bool keyUp(int symbol);
	void appActivate();
	void appDeactivate();
	void draw();
	void update(float dt);
	void resize(const Point2i &s);
	void setGUI(GUI* g);
	void setScene(Scene* s);
	void setOption(const std::string& name, const std::string& value);
	std::string getOption(const std::string& name);
private:
    std::map<std::string, std::string> options;
	GUI* GUIPtr;
	Scene* ScenePtr;
};
#endif
