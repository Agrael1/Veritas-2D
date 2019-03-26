#include "GameDemo.h"
#include "Class.h"
#include "New.h"
#include "Maze.h"

#define base (this->_)

float m_fPlayerX, m_fPlayerY;
int cellL = 3;
unsigned char cellsX = 0, cellsY = 0;
bool key = true;

COORD current; 
COORD next;

void AnimateMaze(void* self)
{
	struct GameDemo* this = self;

	next = current;
	if (key&&this->maze->method->MazeNext(this->maze, &next.X, &next.Y) == 0)
	{

		if ((next.X <= current.X) && (next.Y >= current.Y))
			base.method->Fill(this, next.X*cellL + 1, current.Y*cellL + 1, current.X*cellL + cellL, next.Y*cellL + cellL, L' ', BG_WHITE);
		else if ((next.X >= current.X) && (next.Y <= current.Y))
			base.method->Fill(this, current.X*cellL + 1, next.Y*cellL + 1, next.X*cellL + cellL, current.Y*cellL + cellL, L' ', BG_WHITE);
		else if (next.X <= current.X&&next.Y <= current.Y)
			base.method->Fill(this, next.X*cellL + 1, next.Y*cellL + 1, current.X*cellL + cellL, current.Y*cellL + cellL, L' ', BG_WHITE);
		else
			base.method->Fill(this, current.X*cellL + 1, current.Y*cellL + 1, next.X*cellL + cellL, next.Y*cellL + cellL, L' ', BG_WHITE);

		current = next;
	}
	else
	{
		key = false;
	}
	base.method->Fill(this, current.X*cellL + 1, current.Y*cellL + 1, current.X*cellL + cellL, current.Y*cellL + cellL, L' ', BG_CYAN);
}

void DrawBackground(void* self)
{
	struct GameDemo* this = self;
	
	// Labyrinth cells
	for (int i = 0; i < base.m_nScreenWidth; i += cellL)
		for (int j = 0; j < base.m_nScreenHeight; j += cellL)
			base.method->Fill(this, i + 1, j + 1, i + cellL, j + cellL, L' ', BG_WHITE);
}

void DrawMaze(void* self)
{
	struct GameDemo* this = self;
	// cellL will be the guide
	// req borderL cellL
	//  3
	// ####
	//2#  #0
	// #  #
	// ####
	//  1
	short elctr = 0;
	for (int j = 0; j < cellsY; j++)
		for (int i = !(j & 1); i < cellsX; i += 2, elctr++)
		{
			if (this->maze->MazeRep[elctr] & 1)
				base.method->DrawRectangle(this, i*cellL + cellL, j*cellL + 1, i*cellL + cellL + 1, j*cellL + cellL, BG_WHITE);
			if (this->maze->MazeRep[elctr] & 2)
				base.method->DrawRectangle(this, i*cellL + cellL, j*cellL + cellL + 1, i*cellL + 1, j*cellL + cellL, BG_WHITE);
			if (this->maze->MazeRep[elctr] & 4)
				base.method->DrawRectangle(this, i*cellL, j*cellL + 1, i*cellL + 1, j*cellL + cellL, BG_WHITE);
			if (this->maze->MazeRep[elctr] & 8)
				base.method->DrawRectangle(this, i*cellL+1, j*cellL, i*cellL + cellL, j*cellL + 1, BG_WHITE);
		}
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

	this->maze->method->generateComplete(this->maze);

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

	DrawBackground(this);
	DrawMaze(this);
	// AnimateMaze(this);

	base.method->Fill(this, this->maze->startx*cellL+1, this->maze->starty*cellL + 1, this->maze->startx*cellL + cellL, this->maze->starty*cellL + cellL, L' ', BG_RED);

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
