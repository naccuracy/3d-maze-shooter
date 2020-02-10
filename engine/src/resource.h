#ifndef __RESOURCE_H
#define __RESOURCE_H

class ResourceRef
{
	string name;
	ResourceRef();
	ResourceRef(ResourceRef &rr);
	~ResourceRef();
	int Load();
};

class Texture: public RecourceRef
{
	
};

class Sound: public ResourceRef
{
	
};

class Font: public ResourceRef
{
	
};

class Text: public RecourceRef
{
	
};
#endif