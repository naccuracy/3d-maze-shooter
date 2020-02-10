#include "scene.h"

	virtual void update(float dt){};
	virtual void draw(){};
	virtual bool mouseDown(const Point2i &p);
	virtual void mouseUp(const Point2i &p);
	virtual bool mouseMove(const Point2i &p);
	virtual bool mouseOver(const Point2i &p);
	virtual bool keyDown(int symbol);
	virtual bool keyUp(int symbol);
	virtual void resize(const Point2i &s);
