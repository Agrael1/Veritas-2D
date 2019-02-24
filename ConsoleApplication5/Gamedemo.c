#include "GameDemo.h"
#include "Class.h"

// Creation handling override *unused*
bool OnUserCreate()
{
	return true;
}
// Frame update handling override
bool OnUserUpdate(void* self, float fElapsedTime)
{
	struct GameDemo *this = self;
	for (int x = 0; x < this->_.m_nScreenWidth; x++)
		for (int y = 0; y < this->_.m_nScreenHeight; y++)
			this->_.Printscr(this, x, y, L'#', rand() % 16);
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