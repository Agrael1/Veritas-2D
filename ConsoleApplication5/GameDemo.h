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

static bool OnUserUpdate(void* self, float fElapsedTime)
{
	struct GameDemo *this = self;
	for (int x = 0; x < this->_.m_nScreenWidth; x++)
		for (int y = 0; y < this->_.m_nScreenHeight; y++)
			this->_.Draw(this, x, y, L'#', rand() % 16);
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

