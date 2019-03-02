#include "GameDemo.h"
#include "Class.h"
#include <time.h>



float m_fPlayerX, m_fPlayerY;

struct GameDemo* this;
#define base (this->_)


// Creation handling override
bool OnUserCreate()
{
	
	m_fPlayerX = 10.0f;
	m_fPlayerY = 10.0f;
	return true;
}
// Frame update handling override
bool OnUserUpdate(float fElapsedTime)
{
#pragma region Controls
	if (base.m_keys[VK_LEFT].bHeld)
		m_fPlayerX -= 15.0f*fElapsedTime;
	if (base.m_keys[VK_RIGHT].bHeld)
		m_fPlayerX += 15.0f*fElapsedTime;
	if (base.m_keys[VK_UP].bHeld)
		m_fPlayerY -= 15.0f*fElapsedTime;
	if (base.m_keys[VK_DOWN].bHeld)
		m_fPlayerY += 15.0f*fElapsedTime;
	if (base.m_keys[27].bHeld)
		return false;
#pragma endregion
	
	int cellL = 3;

	Fill(this, 0, 0, base.m_nScreenWidth, base.m_nScreenHeight, L' ', BG_BLACK);

	// Labyrinth cells
	for (int i = 0; i < base.m_nScreenWidth; )
	{
		for (int j = 0; j < base.m_nScreenHeight; )
		{ 
			base.Fill(this, i + 1, j + 1, i + cellL, j + cellL, L' ', BG_CYAN);
			j+=cellL;
		}
		i+= cellL;
	}

	base.Fill(this, (int)m_fPlayerX, (int)m_fPlayerY, (int)m_fPlayerX+5, (int)m_fPlayerY+5, L' ', BG_WHITE);

	return true;
}
// Constructor (must be last)
void* GameDemo_ctor(void* _self, va_list *ap)
{
	this = ((struct Class*)olcGameEngine)->ctor(_self, ap);

	base.OnUserCreate = OnUserCreate;
	base.OnUserUpdate = OnUserUpdate;

	return this;
}
// Destructor
void* GameDemo_dtor(void* self)
{
	this = ((struct Class*)olcGameEngine)->dtor(self);
	return this;
}

static const struct Class _GameDemo = 
{
	sizeof(struct GameDemo), 
	GameDemo_ctor, GameDemo_dtor,0,0
};

const void* GameDemo = &_GameDemo;