#include "Maze.h"
#include "Class.h"
#include "New.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Private Variables
#define private (*(struct _private*)(this->_private))
struct _private
{
	unsigned char SizeInBytes;
	unsigned char* matrix;
	struct Stack* S;
	unsigned char ByteX;
};

unsigned char vector;
unsigned char rotor;
unsigned char rotct, retcond;

void toBinary(const void* self)
{
	struct Maze *this = self;
	struct _private *r = this->_private;
	unsigned char local;
	for (int j = 0; j < 33; j++)		//for the strings of y
	{
		for (int i = 0; i < r->ByteX; i++)		//first bytes cycle (1 2)
		{
			local = *(r->matrix + (j*r->ByteX + i));
			for (int k = 0; k < 8; k++)
			{
				printf("%2d", local % 2);
				local /= 2;
			}
		}
		printf("\n");
	}
}

 #define bitSet(x, y) (private.matrix [(y)*private.ByteX + (x) / 8] |= 1 << ((x) % 8))

// Tests the bit at x,y
unsigned char bitTest(void* self, unsigned char x, unsigned char y)
{
	struct Maze* this = self;
	return *(private.matrix + y*private.ByteX + x / 8) & (1 << (x % 8));
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

	struct _private *r = this->_private;
	
	short t;

	toBinary(this);

	toBinary(private.matrix, 33, private.ByteX);

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
		if ((inx + 1 < this->DimX) && (retcond || !bitTest(this, inx + 1, iny))) { *nx = inx + 1; *ny = iny; break; }
		else goto unres;
	case 0b01:
		if ((iny + 1 < this->DimY) && (retcond || !bitTest(this, inx, iny + 1))) { *nx = inx; *ny = iny+1; break; }
		else goto unres;
	case 0b10:
		if ((inx - 1 >= 0) && (retcond || !bitTest(this, inx - 1, iny))) { *nx = inx - 1; *ny = iny; break; }
		else goto unres;
	case 0b11:
		if ((iny - 1 >= 0) && (retcond || !bitTest(this, inx, iny - 1))) { *nx = inx; *ny = iny-1; break; }
		else goto unres;
	default:								// unreacheable context used to avoid making function with many arguments
	{
	unres:	
		if (rotct < 3)
		{
			vector -= rotor - 1;
			rotor = (rotor + 1) % 3; rotct++;
			goto errhand1;
		}
		else
		{
			if ((t = private.S->method->bPop(private.S, 2)) == -1)
				return 1;
			else
			{
				vector = (unsigned char)(t) ^ 2;
				retcond = 1;
				goto errhand2;
			}
		}
	}
	}	// end of switch

	if (!retcond)
	{
		private.S->method->bPush(private.S, vector, 2);			// if everything's ok push to the stack and return to your business
		r->matrix[ (*ny)*(r->ByteX) + (*nx) / 8] |= 1 << ((*nx) % 8);
	}

	return 0;
}

const vftb __method = { MazeNext };

void* Maze_ctor(void* self, va_list *ap)
{
	struct Maze* this = self;

	this->method = &__method;
	this->DimX = va_arg(*ap, unsigned char);
	this->DimY = va_arg(*ap, unsigned char);

	
		void * p = &(struct _private) 
	{ 
		.SizeInBytes = this->DimX*this->DimY >> 1,
		.S = new(Stack),
		.ByteX = this->DimX % 8 > 0 ? this->DimX / 8 + 1 : this->DimX / 8
	};
		this->_private = p;
	private.matrix = (unsigned char*)malloc(private.ByteX*this->DimY);
	private.S = new(Stack);

	for (int i = 0; i < private.ByteX*this->DimY; i++)
		*(private.matrix + i) = 0;

	srand((unsigned int)time(NULL));
	GetStart(this);			// matrix ptr = 0x00000249225c71e0 0x00007ff84244e6b4

	toBinary(this);

	return this;
}

void* Maze_dtor(void* self)
{
	struct Maze* this = self;
	delete(private.S);
	return this;
}

const struct Class _Maze = { sizeof(struct Maze) ,.ctor = Maze_ctor, .dtor = Maze_dtor};
const void* Maze = &_Maze;


