#ifndef Gamedemo_h
#define Gamedemo_h

#include "olcGameEngine.h"

extern const void* GameDemo;

struct GameDemo
{
	struct olcGameEngine _;
	int x;
};

static bool OnUserCreate()
{
	return true;
}

static bool OnUserUpdate(float fElapsedTime)
{
	return true;
}

static void* GameDemo_ctor(void* self, va_list *ap)
{
	struct GameDemo *this = olcGameEngine_ctor(self,ap);
	
	this->_.OnUserCreate = OnUserCreate;
	this->_.OnUserUpdate = OnUserUpdate;

	return this;
}

#endif

