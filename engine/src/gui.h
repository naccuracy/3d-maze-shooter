#ifndef __GUI_H
#define __GUI_H
#include <string>
#include <map>
#include "points.h"
#include "colors.h"

using namespace std;

class GUIBase
{
	public:
	GUIBase();
	~GUIBase();
	
	Point2i size;
	Point2i pos;
	
	virtual int init(string optionsFile);
	virtual void release();
	virtual bool mouseDown(const Point2i &p);
	virtual void mouseUp(const Point2i &p);
	virtual bool mouseMove(const Point2i &p);
	virtual bool mouseOver(const Point2i &p);
	virtual bool keyDown(int symbol);
	virtual bool keyUp(int symbol);
	virtual void draw();
	virtual void update(float dt);
	virtual void reize(const Point2i &s);
	virtual bool hitTest(const Point2i &p);
		
	map<string, GUIBase*> childs;
};

class GUIElement: public GUIBase
{
	public:
	
	GUIElement();
	GUIElement(GUIElement* prnt);
	GUIElement(const GUIElement& element);
	~GUIElement();
	virtual void draw();
	void hide();
	void show();
	bool isVisible();
	bool hasFocus();
	void setFocus(bool on);
	
	private:
	string name;
	bool hasFocus;
	bool visible;
	Color4f color;
	GUIElement *parent;
	
};
/*
class TextEdit: public GUIElement
{
	public:
	string text;
};

class TextLabel: public GUIElement
{
	public:
	string text;
};

class Button: public GUIElement
{
	public:
	string text;	
};

class Form: public GUIElement
{
	public:
	string caption;
};

class MenuList: public GUIElement
{
	public:
	vector<string> items;
};

class Slider: public GUIElement
{
	public:
	float value;//0-1
};

class ProgressBar: public GUIElement
{
	public:
	float value;//0-1
};

class MarkableItem: public GUIElement //checkBox
{
	public:
	bool checked;
};
* */
#endif
