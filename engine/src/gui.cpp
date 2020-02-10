#include "gui.h"

#include <iostream>
#include "GL/glu.h"
using namespace std;

GUIBase::GUIBase()
:size(256, 256)
,pos(0, 0)
{
    cout << "GUIBase::GUIBase()" << endl;
}

GUIBase::~GUIBase()
{
    cout << "GUIBase::~GUIBase()" << endl;
    for(auto it: childs)
    {
		if(it.second){
			delete it.second;
			it.second = NULL;
		}
	}
	childs.clear();
}
	
int GUIBase::init(string optionsFile)
{
    cout << "GUIBase::init" << endl;
}
void GUIBase::release()
{
    cout << "GUIBase::release" << endl;
}
bool GUIBase::hitTest(const Point2i &p)
{
	return ((p.x >= pos.x) 
	&& (p.x <= (pos.x + size.x)) 
	&& (p.y >= pos.y) 
	&& (p.y <= (pos.y + size.y)));
}
bool GUIBase::mouseDown(const Point2i &p)
{
	Point2i childsp = p - pos;
	bool res = false;
    cout << "GUIBase::mouseDown" << endl;
    if(hitTest(p))
    {
		res = true;
		for(auto it: childs)
		{
			if(it.second->hitTest(childsp)){
				//it.second->setFocus(true);
				it.second->mouseDown(childsp);
			}
		}
	}
	return res;
}
void GUIBase::mouseUp(const Point2i &p)
{
    cout << "GUIBase::mouseUp" << endl;
    Point2i childsp = p - pos;
    if(hitTest(p))
    {
		for(auto it: childs)
		{
			if(it.second->hitTest(childsp)){
				it.second->setFocus(true);
				it.second->mouseUp(childsp);
			}
		}
	}
}
bool GUIBase::mouseMove(const Point2i &p)
{
    cout << "GUIBase::mouseMove()" << endl;
    Point2i childsp = p - pos;
	bool res = false;
    if(hitTest(p))
    {
		res = true;
		for(auto it: childs)
		{
			if(it.second->hitTest(childsp)){
				it.second->mouseMove(childsp);
			}
		}
	}
	return res;
}
bool GUIBase::mouseOver(const Point2i &p)//??
{
    cout << "GUIBase::mouseOver()" << endl;
}
void GUIBase::keyDown(int symbol)
{
    cout << "GUIBase::keyDown()" << endl;
    for(auto it: childs)
	{
		if(it.second->hasFocus()){
			it.second->keyDown(symbol);
		}
	}
}
void GUIBase::keyUp(int symbol)
{
    cout << "GUIBase::keyUp()" << endl;
    for(auto it: childs)
	{
		if(it.second->hasFocus()){
			it.second->keyDown(symbol);
		}
	}
}
void GUIBase::draw()
{
    //cout << "GUIBase::draw()" << endl;
    //GUI draw on 2D plane with size as window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,(GLdouble)size.x, 0, (GLdouble)size.y);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(pos.x, pos.y, 0);
    //draw all visible childs
    GUIElement* el;
    for(auto it: childs)
    {
		el = it.second;
		if(el->isVisible){
			el->draw();
		}
	}
}
void GUIBase::update(float dt)
{
    cout << "GUIBase::update()" << endl;
}
void GUIBase::reize(const Point2i &s)
{
    cout << "GUIBase::reize()" << endl;
    size = s;
}


///GUIElement
GUIElement::GUIElement()
{
	parent =NULL;
}
GUIElement::GUIElement(GUIElement* prnt)
{
	parent = prnt;
}
GUIElement::GUIElement(const GUIElement& element);
GUIElement::~GUIElement()
{
	parent = NULL;
	for(auto it: childs)
    {
		if(it.second){
			delete it.second;
			it.second = NULL;
		}
	}
	childs.clear();
}
void GUIElement::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(pos.x, pos.y, 0);
	glColor4f(color.r, color.g, color.b, color.a);
	glRectf(pos.x, pos.y, pos.x + size.x, pos.y +size.y);
	for(auto it: childs)
    {
		if(it.second->isVisible()){
			it.second->draw();
		}
	}
	glPopMatrix();
}
void GUIElement::hide()
{
    cout << "GUIElement::hide()" << endl;
    visible = false;
}
void GUIElement::show()
{
    cout << "GUIElement::show()" << endl;
    visible = true;
}
bool GUIElement::isVisible()
{
	return visible;
}
bool GUIElement::hasFocus()
{
	return hasFocus;
}
void GUIElement::setFocus(bool on)
{
	hasFocus = on;
}
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
*/
