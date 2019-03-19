#include "GameDemo.h"
#include "Class.h"
#include "New.h"
#include "Maze.h"

float m_fPlayerX, m_fPlayerY;
int cellL = 3;
unsigned char cellsX = 0, cellsY = 0;
bool key = true;


COORD current; 
COORD next;

#define base (this->_)


void DrawBackground(void* self)
{
	struct GameDemo* this = self;

	base.method->Fill(this, 0, 0, base.m_nScreenWidth, base.m_nScreenHeight, L' ', BG_BLACK);

	// Labyrinth cells
	for (int i = 0; i < base.m_nScreenWidth; i += cellL)
		for (int j = 0; j < base.m_nScreenHeight; j += cellL)
			base.method->Fill(this, i + 1, j + 1, i + cellL, j + cellL, L'&', BG_CYAN);
}
// Creation handling override
bool OnUserCreate(void* self)
{
	struct GameDemo* this = self;

	cellsY = base.m_nScreenHeight / cellL;
	cellsX = base.m_nScreenWidth / cellL;

	this->maze = new(Maze, cellsX, cellsY);
	current.X = this->maze->startx;
	current.Y = this->maze->starty;

	

	m_fPlayerX = 10.0f;
	m_fPlayerY = 10.0f;
	return true;
}
// Frame update handling override
bool OnUserUpdate(void* self, float fElapsedTime)
{
	struct GameDemo* this = self;
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


	//DrawBackground(this);

	if (key&&this->maze->method->MazeNext(this->maze, current.X, current.Y, &next.X, &next.Y)==0)
	{
		if ((next.X <= current.X)&&(next.Y >= current.Y))
			base.method->Fill(this, next.X*cellL + 1, current.Y*cellL + 1, current.X*cellL + cellL, next.Y*cellL + cellL, L' ', BG_WHITE);
		else if ((next.X>=current.X)&&(next.Y<=current.Y))
			base.method->Fill(this, current.X*cellL + 1, next.Y*cellL + 1, next.X*cellL + cellL, current.Y*cellL + cellL, L' ', BG_WHITE);
		else if (next.X<=current.X&&next.Y<=current.Y)
			base.method->Fill(this, next.X*cellL + 1, next.Y*cellL + 1, current.X*cellL + cellL, current.Y*cellL + cellL, L' ', BG_WHITE);
		else
			base.method->Fill(this, current.X*cellL+1, current.Y*cellL+1, next.X*cellL+cellL, next.Y*cellL + cellL, L' ', BG_WHITE);

		current = next;
	}
	else
	{
		key = false;
	}

	base.method->Fill(this, this->maze->startx*cellL+1, this->maze->starty*cellL + 1, this->maze->startx*cellL + cellL, this->maze->starty*cellL + cellL, L' ', BG_RED);
	base.method->Fill(this, current.X*cellL + 1, current.Y*cellL + 1, current.X*cellL + cellL, current.Y*cellL + cellL, L' ', BG_CYAN);

	base.method->Fill(this, (int)m_fPlayerX, (int)m_fPlayerY, (int)m_fPlayerX + 5, (int)m_fPlayerY + 5, L' ', BG_WHITE);

	return true;
}
// Constructor (must be last)
void* GameDemo_ctor(void* _self, va_list *ap)
{
	struct GameDemo* this = ((struct Class*)olcGameEngine)->ctor(_self, ap);

	base.method->OnUserCreate = OnUserCreate;
	base.method->OnUserUpdate = OnUserUpdate;

	return this;
}
// Destructor
void* GameDemo_dtor(void* self)
{
	struct GameDemo* this = ((struct Class*)olcGameEngine)->dtor(self);

	delete(this->maze);
	return this;
}

static const struct Class _GameDemo =
{
	sizeof(struct GameDemo),
	GameDemo_ctor, GameDemo_dtor,0,0
};

const void* GameDemo = &_GameDemo;
