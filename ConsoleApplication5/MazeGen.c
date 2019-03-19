#include "Maze.h"
#include "Class.h"
#include "New.h"
#include <stdlib.h>
#include <time.h>

// Internal Variables
unsigned char ByteX;
unsigned char* matrix;
struct Stack* S;
unsigned char vector;

unsigned char rotor;
unsigned char rotct, retcond;

void bitSet(unsigned char x, unsigned char y)
{
	*(matrix + y*ByteX + x / 8) |= 1 << (x % 8);
}
// Tests the bit at x,y
unsigned char bitTest(unsigned char x, unsigned char y)
{
	return *(matrix + y*ByteX + x / 8) & (1 << (x % 8));
}


// 
void GetStart(void* self)
{
	struct Maze* this = self;
	switch (rand() % 4) {
	case 0:
	{
		this->startx = rand() % this->DimX;
		this->starty = 0;
		vector = 0b01; break;
	}
	case 1:
	{
		this->startx = rand() % this->DimX;
		this->starty = this->DimY - 1;
		vector = 0b11; break;
	}
	case 2:
	{
		this->starty = rand() % this->DimY;
		this->startx = 0;
		vector = 0b00; break;
	}
	case 3:
	{
		this->starty = rand() % this->DimY;
		this->startx = this->DimX - 1;
		vector = 0b10; break;
	}
	}
	bitSet(this->startx, this->starty);
}

int MazeNext(void* self, unsigned char inx, unsigned char iny, unsigned char *nx, unsigned char *ny)
{
	struct Maze* this = self;
	short t;

	// flush rotor counter and return condition
	rotct = 0;
	retcond = 0;

	// represent a vector rotation
	rotor = rand() % 3;

errhand1:
	vector += rotor - 1;
errhand2:// this is the part in which we are taken over direction control manually, but it can work automatically as well

	// Scanning the next position counted by rotor
	switch (vector %= 4)
	{
	case 0b00:
		if ((inx + 1 < this->DimX) && (retcond || !bitTest(inx + 1, iny))) { *nx = inx + 1; *ny = iny; break; }
		else goto unres;
	case 0b01:
		if ((iny + 1 < this->DimY) && (retcond || !bitTest(inx, iny + 1))) { *nx = inx; *ny = iny+1; break; }
		else goto unres;
	case 0b10:
		if ((inx - 1 >= 0) && (retcond || !bitTest(inx - 1, iny))) { *nx = inx - 1; *ny = iny; break; }
		else goto unres;
	case 0b11:
		if ((iny - 1 >= 0) && (retcond || !bitTest(inx, iny - 1))) { *nx = inx; *ny = iny-1; break; }
		else goto unres;
	default:								// unreacheable context used to avoid making function with many arguments
	unres:	if (rotct < 3)
	{
		vector -= rotor - 1;
		rotor = (rotor + 1) % 3; rotct++;
		goto errhand1;
	}
			else
			{
				if((t = S->method->bPop(S, 2)) ==-1)
				{
					free(matrix);
					return 1;
				}
				else 
				{
					vector = (unsigned char)(t)^2;
					retcond = 1;
					goto errhand2;
				}
			}
	}
	if (!retcond)
	{
		S->method->bPush(S, vector, 2);			// if everything's ok push to the stack and return to your business
		bitSet(*nx, *ny);
	}
	return 0;
}

const vftb __method = { MazeNext };

void* Maze_ctor(void* self, va_list *ap)
{
	struct Maze* this = self;

	S = new(Stack);
	
	this->method = &__method;
	this->DimX = va_arg(*ap, unsigned char);
	this->DimY = va_arg(*ap, unsigned char);

	ByteX = this->DimX % 8 > 0 ? this->DimX / 8 + 1 : this->DimX / 8;

	matrix = (unsigned char*)malloc(ByteX*this->DimY);

	for (int i = 0; i < ByteX*this->DimY; i++)
		*(matrix + i) = 0;

	srand((unsigned int)time(NULL));
	GetStart(this);

	return this;
}

void* Maze_dtor(void* self)
{
	struct Maze* this = self;
	delete(S);
	return this;
}

const struct Class _Maze = { sizeof(struct Maze) ,.ctor = Maze_ctor, .dtor = Maze_dtor};
const void* Maze = &_Maze;