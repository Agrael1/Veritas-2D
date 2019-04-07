#include <math.h>
#include "Class.h"

#include "New.h"
#include "Maze.h"
#include "GameDemo.h"

#define base (this->_)

void Test(void* self)
{
	struct c_class* this = self;

	for (int x = 0; x < base.m_nScreenWidth; x++)
	{
	// Divides FOV by 2 and project a ray to World Space
		float fRayAngle = (private.m_fPlayerA - this->fFOV / 2.0f) + ((float)x / (float)base.m_nScreenWidth)*this->fFOV;

		float fDistancetoWall = 0;
		bool bHitWall = false;

		float fEyeX = sinf(fRayAngle);
		float fEyeY = cosf(fRayAngle);

		while (!bHitWall&&fDistancetoWall<this->fDepth)
		{
			fDistancetoWall += 0.1f;

			int nTestX = (int)(private.m_fPlayerX + fEyeX*fDistancetoWall);
			int nTestY = (int)(private.m_fPlayerY + fEyeY*fDistancetoWall);

			if (nTestX < 0 || nTestX >= this->localFrame->nFrameLength || nTestY < 0 || nTestX >= this->localFrame->nFrameHeight)
			{
				bHitWall = true;
				fDistancetoWall = this->fDepth;
			}
			else
			{
				//Raytest
				if (this->localFrame->localFrame[nTestY*this->localFrame->nFrameLength + nTestX].Attributes==BG_BLACK)
				{
					bHitWall = true;
				}
			}
		}
		//Calculate perspective
		int nCeiling =(int)((float)(base.m_nScreenHeight / 2.0) - base.m_nScreenHeight / ((float)fDistancetoWall));
		int nFloor = base.m_nScreenHeight - nCeiling;

		short nShade = ' ';
		short nFloorShade = ' ';

		if (fDistancetoWall <= this->fDepth / 4.0f)		nShade = 0x2588;
		else if (fDistancetoWall <= this->fDepth / 3.0f)	nShade = 0x2593;
		else if (fDistancetoWall <= this->fDepth / 2.0f)	nShade = 0x2592;
		else if (fDistancetoWall <= this->fDepth)			nShade = 0x2591;
		else										nShade = ' ';

		for (int y = 0; y < base.m_nScreenHeight; y++)
		{
			if (y < nCeiling)
			{
				base.method->PrintChar(this, x, y, L' ', FG_WHITE);
			}
			else if (y > nCeiling&&y <= nFloor)
			{
				base.method->PrintChar(this, x, y, nShade, FG_DARK_CYAN);
			}
			else
			{
				// Shade floor
				float b = 1.0f - (((float)y - base.m_nScreenHeight / 2.0f) / ((float)base.m_nScreenHeight / 2.0f));
				if (b < 0.25)			nFloorShade = 0x2592;
				else if (b < 0.5)		nFloorShade = 0x2592;
				else if (b < 0.75)		nFloorShade = '.';
				else if (b < 0.9)		nFloorShade = '-';
				else					nFloorShade = ' ';
				base.method->PrintChar(this, x, y, nFloorShade, FG_BLACK+BG_DARK_RED);
			}
		}
	}

	base.method->Compose(this, this->localFrame, 0, 0);

	base.method->PrintChar(this, (int)private.m_fPlayerX,((int)private.m_fPlayerY),  ' ', BG_MAGENTA);
}

void AnimateMaze(void* self)
{
	struct GameDemo* this = self;

	private.next = private.current;
	if (private.key&&this->maze->method->MazeNext(this->maze, &private.next.X, &private.next.Y) == 0)
	{

		if ((private.next.X <= private.current.X) && (private.next.Y >= private.current.Y))
			base.method->Fill(this, private.next.X*this->CellL + 1, private.current.Y*this->CellL + 1, private.current.X*this->CellL + this->CellL, private.next.Y*this->CellL + this->CellL, L' ', BG_WHITE);
		else if ((private.next.X >= private.current.X) && (private.next.Y <= private.current.Y))
			base.method->Fill(this, private.current.X*this->CellL + 1, private.next.Y*this->CellL + 1, private.next.X*this->CellL + this->CellL, private.current.Y*this->CellL + this->CellL, L' ', BG_WHITE);
		else if (private.next.X <= private.current.X&&private.next.Y <= private.current.Y)
			base.method->Fill(this, private.next.X*this->CellL + 1, private.next.Y*this->CellL + 1, private.current.X*this->CellL + this->CellL, private.current.Y*this->CellL + this->CellL, L' ', BG_WHITE);
		else
			base.method->Fill(this, private.current.X*this->CellL + 1, private.current.Y*this->CellL + 1, private.next.X*this->CellL + this->CellL, private.next.Y*this->CellL + this->CellL, L' ', BG_WHITE);

		private.current = private.next;
	}
	else
	{
		private.key = false;
	}
	base.method->Fill(this, private.current.X*this->CellL + 1, private.current.Y*this->CellL + 1, private.current.X*this->CellL + this->CellL, private.current.Y*this->CellL + this->CellL, L' ', BG_CYAN);
}

void DrawBackground(void* self)
{
	struct c_class* this = self;
	
	// Labyrinth cells
	for (Byte j = 0; j<this->cellsY;j++)
		for (Byte i = 0; i < this->cellsX; i++)
			this->localFrame->method->DrawRectangle(this->localFrame, i*this->CellL+1, j*this->CellL+1, i*this->CellL+this->CellL, j*this->CellL+this->CellL, BG_WHITE);
}

void DrawMaze(void* self)
{
	struct c_class* this = self;

	short elctr = 0;
	for (int j = 0; j < this->cellsY; j++)
		for (int i = !(j & 1); i < this->cellsX; i += 2, elctr++)
		{
			if (this->maze->MazeRep[elctr] & 1)
				this->localFrame->method->DrawRectangle(this->localFrame, i*this->CellL + this->CellL, j*this->CellL + 1, i*this->CellL + this->CellL + 1, j*this->CellL + this->CellL, BG_WHITE);
			if (this->maze->MazeRep[elctr] & 2)
				this->localFrame->method->DrawRectangle(this->localFrame, i*this->CellL + this->CellL, j*this->CellL + this->CellL + 1, i*this->CellL + 1, j*this->CellL + this->CellL, BG_WHITE);
			if (this->maze->MazeRep[elctr] & 4)
				this->localFrame->method->DrawRectangle(this->localFrame, i*this->CellL, j*this->CellL + 1, i*this->CellL + 1, j*this->CellL + this->CellL, BG_WHITE);
			if (this->maze->MazeRep[elctr] & 8)
				this->localFrame->method->DrawRectangle(this->localFrame, i*this->CellL+1, j*this->CellL, i*this->CellL + this->CellL, j*this->CellL + 1, BG_WHITE);
		}
}

// Creation handling override
bool OnUserCreate(void* self)
{
	struct c_class* this = self;

	this->cellsY = 10;
	this->cellsX = 10;

	this->localFrame = new(Frame, (this->cellsX*this->CellL + 1), (this->cellsY*this->CellL + 1));

	this->maze = new(Maze, this->cellsX, this->cellsY);
		
	this->fDepth = 16;
	private.current.X = this->maze->startx;
	private.current.Y = this->maze->starty;
	this->maze->method->generateComplete(this->maze);

	DrawBackground(this);
	DrawMaze(this);

	// Player and game stuff

	private.m_fPlayerX = 1;
	private.m_fPlayerY = 1;
	this->fFOV = (float)M_PI / 4;

	return true;
}
// Frame update handling override
bool OnUserUpdate(void* self, float fElapsedTime)
{
	struct c_class* this = self;

#pragma region Controls

	if (base.m_keys['A'].bHeld)
		private.m_fPlayerA -= (1.0f)*fElapsedTime;
	if (base.m_keys['D'].bHeld)
		private.m_fPlayerA += (1.0f)*fElapsedTime;


	private.m_fPlayerA += (base.m_mousePosX-base.m_nScreenWidth/2)*fElapsedTime*0.02;

	if (base.m_keys['W'].bHeld)
	{
		private.m_fPlayerX += sinf(private.m_fPlayerA)*(5.0f)*fElapsedTime;
		private.m_fPlayerY += cosf(private.m_fPlayerA)*(5.0f)*fElapsedTime;

		if (this->localFrame->localFrame[(int)private.m_fPlayerY*this->localFrame->nFrameLength + (int)private.m_fPlayerX].Attributes == BG_BLACK)
		{
			private.m_fPlayerX -= sinf(private.m_fPlayerA)*(5.0f)*fElapsedTime;
			private.m_fPlayerY -= cosf(private.m_fPlayerA)*(5.0f)*fElapsedTime;
		}
	}
	if (base.m_keys['S'].bHeld)
	{
		private.m_fPlayerX -= sinf(private.m_fPlayerA)*(5.0f)*fElapsedTime;
		private.m_fPlayerY -= cosf(private.m_fPlayerA)*(5.0f)*fElapsedTime;

		if (this->localFrame->localFrame[(int)private.m_fPlayerY*this->localFrame->nFrameLength + (int)private.m_fPlayerX].Attributes == BG_BLACK)
		{
			private.m_fPlayerX += sinf(private.m_fPlayerA)*(5.0f)*fElapsedTime;
			private.m_fPlayerY += cosf(private.m_fPlayerA)*(5.0f)*fElapsedTime;
		}
	}

	if (base.m_keys[27].bHeld)
		return false;
#pragma endregion

	// AnimateMaze(this);

	Test(this);

	base.method->Fill(this, this->maze->startx*this->CellL+1, this->maze->starty*this->CellL + 1, this->maze->startx*this->CellL + this->CellL, this->maze->starty*this->CellL + this->CellL, L' ', BG_RED);

	if((int)private.m_fPlayerX == this->maze->startx*this->CellL + 1 &&(int)private.m_fPlayerY == this->maze->starty*this->CellL+1)
		return false;
	return true;
}
// Constructor (must be last)
void* GameDemo_ctor(void* _self, va_list *ap)
{
	struct c_class* this = ((struct Class*)olcGameEngine)->ctor(_self, ap);

	base.method->OnUserCreate = OnUserCreate;
	base.method->OnUserUpdate = OnUserUpdate;

	private.key = true;
	this->CellL = 3;

	return this;
}
// Destructor
void* GameDemo_dtor(void* self)
{
	struct c_class* this = ((struct Class*)olcGameEngine)->dtor(self);

	delete(this->maze);
	return this;
}

static const struct Class _GameDemo =
{
	sizeof(struct c_class),
	GameDemo_ctor, GameDemo_dtor,0,0
};

const void* GameDemo = &_GameDemo;