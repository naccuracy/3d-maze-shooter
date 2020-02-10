#ifndef __SCENE_H
#define __SCENE_H
#include "points.h"
//TODO add funcs from app
//add camera & models lights etc
class Scene
{
public:
	Scene(){};
	~Scene(){};
	virtual void update(float dt){};
	virtual void draw(){};
	virtual bool mouseDown(const Point2i &p);
	virtual void mouseUp(const Point2i &p);
	virtual bool mouseMove(const Point2i &p);
	virtual bool mouseOver(const Point2i &p);
	virtual bool keyDown(int symbol);
	virtual bool keyUp(int symbol);
	virtual void resize(const Point2i &s);
};
#endif
