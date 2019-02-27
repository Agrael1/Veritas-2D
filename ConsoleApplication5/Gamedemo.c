#include "GameDemo.h"
#include "Class.h"

float m_fPlayerX, m_fPlayerY;

// Creation handling override
bool OnUserCreate(void* self)
{
	m_fPlayerX = 10.0f;
	m_fPlayerY = 10.0f;
	return true;
}
// Frame update handling override
bool OnUserUpdate(void* self, float fElapsedTime)
{
	struct GameDemo *this = self;

	if (this->_.m_keys[VK_LEFT].bHeld)
		m_fPlayerX -= 15.0f*fElapsedTime;
	if (this->_.m_keys[VK_RIGHT].bHeld)
		m_fPlayerX += 15.0f*fElapsedTime;
	if (this->_.m_keys[VK_UP].bHeld)
		m_fPlayerY -= 15.0f*fElapsedTime;
	if (this->_.m_keys[VK_DOWN].bHeld)
		m_fPlayerY += 15.0f*fElapsedTime;

	this->_.Fill(this, 0, 0, this->_.m_nScreenWidth, this->_.m_nScreenHeight, L' ', 0);
	this->_.FillCenter(this, 7, 7, L'&', BG_CYAN);
	this->_.Fill(this, (int)m_fPlayerX, (int)m_fPlayerY, (int)m_fPlayerX+5, (int)m_fPlayerY+5, L' ', BG_WHITE);
	return true;
}
// Constructor (must be last)
void* GameDemo_ctor(void* self, va_list *ap)
{
	struct GameDemo *this = ((struct Class*)olcGameEngine)->ctor(self, ap);

	this->_.OnUserCreate = OnUserCreate;
	this->_.OnUserUpdate = OnUserUpdate;

	return this;
}
// Destructor
void* GameDemo_dtor(void* self)
{
	struct GameDemo *this = ((struct Class*)olcGameEngine)->dtor(self);
	return this;
}

static const struct Class _GameDemo = 
{
	sizeof(struct GameDemo), 
	GameDemo_ctor, GameDemo_dtor
};

const void* GameDemo = &_GameDemo;