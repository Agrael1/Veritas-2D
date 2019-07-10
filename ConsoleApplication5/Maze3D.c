#include "Maze3D.h"
#include "Class.h"
#include <math.h>


#define CellL 3
// Player statics
float fPlayerX, fPlayerY;
double fPlayerA;


void _RenderMaze(void* self, struct Frame* to)
{
	account(self);
	
	for (int x = 0; x < to->nFrameLength; x++)
	{
		// Divides FOV by 2 and project a ray to World Space
		float fRayAngle = (fPlayerA - this->fFOV / 2.0f) + ((float)x / (float)to->nFrameLength)*this->fFOV;

		float fDistancetoWall = 0;
		bool bHitWall = false;

		float fEyeX = sinf(fRayAngle);
		float fEyeY = cosf(fRayAngle);

		while (!bHitWall&&fDistancetoWall<this->fDepth)
		{
			fDistancetoWall += 0.1f;

			int nTestX = (int)(fPlayerX + fEyeX*fDistancetoWall);
			int nTestY = (int)(fPlayerY + fEyeY*fDistancetoWall);

			if (nTestX < 0 || nTestX >= this->MapLayer->nFrameLength || nTestY < 0 || nTestX >= this->MapLayer->nFrameHeight)
			{
				bHitWall = true;
				fDistancetoWall = this->fDepth;
			}
			else
			{
				//Raytest
				if (this->MapLayer->localFrame[nTestY*this->MapLayer->nFrameLength + nTestX].Attributes == BG_BLACK)
				{
					bHitWall = true;
				}
			}
		}

		//Calculate perspective
		int nCeiling = (int)((float)(to->nFrameHeight / 2.0) - to->nFrameHeight / fDistancetoWall);
		int nFloor = to->nFrameHeight - nCeiling;

		short nShade = ' ';
		short nFloorShade = ' ';

		if (fDistancetoWall <= this->fDepth / 4.0f)		nShade = 0x2588;
		else if (fDistancetoWall <= this->fDepth / 3.0f)	nShade = 0x2593;
		else if (fDistancetoWall <= this->fDepth / 2.0f)	nShade = 0x2592;
		else if (fDistancetoWall <= this->fDepth)			nShade = 0x2591;
		else										nShade = ' ';

		for (int y = 0; y < to->nFrameHeight; y++)
		{
			if (y < nCeiling)
			{
				to->method->PrintFrame(to, x, y, L' ', FG_WHITE);
			}
			else if (y > nCeiling&&y <= nFloor)
			{
				to->method->PrintFrame(to, x, y, nShade, FG_DARK_CYAN);
			}
			else
			{
				// Shade floor
				float b = 1.0f - (((float)y - to->nFrameHeight / 2.0f) / ((float)to->nFrameHeight / 2.0f));
				if (b < 0.25)			nFloorShade = 0x2592;
				else if (b < 0.5)		nFloorShade = 0x2592;
				else if (b < 0.75)		nFloorShade = '.';
				else if (b < 0.9)		nFloorShade = '-';
				else					nFloorShade = ' ';
				to->method->PrintFrame(to, x, y, nFloorShade, FG_BLACK + BG_DARK_RED);
			}
		}
	}
}
void _RenderMap(void* self, struct Frame* to)
{
	account(self);

	// Labyrinth cells
	for (Byte j = 0; j<this->MazeWidth; j++)
		for (Byte i = 0; i < this->MazeLength; i++)
			to->method->DrawRectangle(to, i*CellL + 1, j*CellL + 1, (i+1)*CellL, (j+1)*CellL, BG_WHITE);

	// Break generated walls
	short elctr = 0;
	for (int j = 0; j < this->MazeWidth; j++)
		for (int i = !(j & 1); i < this->MazeLength; i += 2, elctr++)
		{
			if (this->maze->MazeRep[elctr] & 1)
				to->method->DrawRectangle(to, (i+1)*CellL, j*CellL + 1, (i+1)*CellL + 1, (j+1)*CellL, BG_WHITE);
			if (this->maze->MazeRep[elctr] & 2)
				to->method->DrawRectangle(to, (i+1)*CellL, (j+1)*CellL + 1, i*CellL + 1, (j+1)*CellL, BG_WHITE);
			if (this->maze->MazeRep[elctr] & 4)
				to->method->DrawRectangle(to, i*CellL, j*CellL + 1, i*CellL + 1, (j+1)*CellL, BG_WHITE);
			if (this->maze->MazeRep[elctr] & 8)
				to->method->DrawRectangle(to, i*CellL + 1, j*CellL, (i+1)*CellL, j*CellL + 1, BG_WHITE);
		}
	//Finish position
	to->method->DrawRectangle(to, this->maze->startx*CellL + 1, this->maze->starty*CellL + 1, (this->maze->startx+1)*CellL, (this->maze->starty+1)*CellL, BG_RED);
	

}
void virtual(HandleMouse)(void* self, struct Mouse* mouse, const double fElapsedTime)
{

	int X; mouse->method->ReadMouseMovement(mouse,&X, NULL);
	if (mouse->method->ButtonPressed(mouse, LEFT_MB))
		X = X / 4;

	fPlayerA += X*fElapsedTime;
}
void virtual(HandleControls)(void* self, const struct Keyboard* kbd, const double fElapsedTime)
{
	account(self);

	if (kbd->method->KeyPressed(kbd, 'W'))
	{
		fPlayerX += sinf(fPlayerA)*(3.0f)*fElapsedTime;
		fPlayerY += cosf(fPlayerA)*(3.0f)*fElapsedTime;

		if (this->MapLayer->localFrame[(int)fPlayerY*this->MapLayer->nFrameLength + (int)fPlayerX].Attributes == BG_BLACK)
		{
			fPlayerX -= sinf(fPlayerA)*(3.0f)*fElapsedTime;
			fPlayerY -= cosf(fPlayerA)*(3.0f)*fElapsedTime;
		}
	}
	if (kbd->method->KeyPressed(kbd, 'S'))
	{
		fPlayerX -= sinf(fPlayerA)*(3.0f)*fElapsedTime;
		fPlayerY -= cosf(fPlayerA)*(3.0f)*fElapsedTime;

		if (this->MapLayer->localFrame[(int)fPlayerY*this->MapLayer->nFrameLength + (int)fPlayerX].Attributes == BG_BLACK)
		{
			fPlayerX += sinf(fPlayerA)*(3.0f)*fElapsedTime;
			fPlayerY += cosf(fPlayerA)*(3.0f)*fElapsedTime;
		}
	}
	if (kbd->method->KeyPressed(kbd, 'A'))
	{
		fPlayerY += sinf(fPlayerA)*(3.0f)*fElapsedTime;
		fPlayerX -= cosf(fPlayerA)*(3.0f)*fElapsedTime;

		if (this->MapLayer->localFrame[(int)fPlayerY*this->MapLayer->nFrameLength + (int)fPlayerX].Attributes == BG_BLACK)
		{
			fPlayerY -= sinf(fPlayerA)*(3.0f)*fElapsedTime;
			fPlayerX += cosf(fPlayerA)*(3.0f)*fElapsedTime;
		}
	}
	if (kbd->method->KeyPressed(kbd, 'D'))
	{
		{
			fPlayerY -= sinf(fPlayerA)*(2.0f)*fElapsedTime;
			fPlayerX += cosf(fPlayerA)*(2.0f)*fElapsedTime;

			if (this->MapLayer->localFrame[(int)fPlayerY*this->MapLayer->nFrameLength + (int)fPlayerX].Attributes == BG_BLACK)
			{
				fPlayerY += sinf(fPlayerA)*(2.0f)*fElapsedTime;
				fPlayerX -= cosf(fPlayerA)*(2.0f)*fElapsedTime;
			}
		}
	}
}
bool virtual(HandleInputEvents)(void* self, struct KeyboardEvent event)
{
	account(self);
	if (event.method->IsPress(&event))
	{
		// check if the event was for the space key
		switch (event.method->GetCode(&event))
		{
		case VK_ESCAPE:
			return false;
		case 'M':
			this->bShowMap ^= true;
			break;
		}
	}
	return true;
}
bool virtual(OnUserCreate)(void* self)
{
	account(self);
	this->MazeLength = 10u;
	this->MazeWidth = 10u;
	this->fDepth = 16;
	this->fFOV = (float)M_PI / 4;

	DWord MapLength = this->MazeLength * 3 + 1;
	DWord MapHeight = this->MazeWidth * 3 + 1;

	this->MapLayer = new(Frame, MapLength, MapHeight);
	this->maze = new(Maze, this->MazeLength, this->MazeWidth);
	this->maze->method->Generate(this->maze);
	
	_RenderMap(this, this->MapLayer);

	this->bShowMap = false;

	fPlayerX = 10;
	fPlayerY = 10;
	fPlayerA = 0;

	return true;
}
bool virtual(OnUserUpdate)(void* self)
{
	account(self);
	// Render Maze
	_RenderMaze(this, base.Output);

	// 'M' toggles the map
	if (this->bShowMap)
	{
		// Emplace Map 
		//base.Output->method->Compose(base.Output, this->MapLayer, 5, 5);
		// Player position
		base.Output->method->PrintFrame(base.Output, (int)fPlayerX + 5, ((int)fPlayerY) + 5, ' ', BG_MAGENTA);
	}

	return true;
}

Constructor(void* self, va_list *ap)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->ctor(self, ap);
	base.AppName = stringOf(self);
	base.method->OnUserCreate = virtual(OnUserCreate);
	base.method->OnUserUpdate = virtual(OnUserUpdate);
	base.method->HandleInputEvents = virtual(HandleInputEvents);
	base.method->HandleControls = virtual(HandleControls);
	base.method->HandleMouse = virtual(HandleMouse);

	return this;
}
Destructor(void* self)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->dtor(self);
	if (this->MapLayer)
		delete(this->MapLayer);
	if (this->maze)
		delete(this->maze);
	return this;
}
ENDCLASSDESC