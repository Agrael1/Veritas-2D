#include "Maze.h"
#include "Class.h"
#include "New.h"
#include <stdlib.h>
#include <time.h>

// Private Variables
#define private (*(Maze_private*)(this->__))

#define bitSet(x, y) (private.matrix [(y)*private.ByteX + (x) / 8] |= 1 << ((x) % 8))
#define bitTest(x, y) (*(private.matrix + (y)*private.ByteX + (x) / 8) & (1 << ((x) % 8)))

// Gets random start point values
void GetStart(struct Maze* this)
{
	switch (rand() % 4) {
	case 0:
	{
		this->startx = rand() % this->DimX;
		this->starty = 0;
		private.vector = 0b01; break;
	}
	case 1:
	{
		this->startx = rand() % this->DimX;
		this->starty = this->DimY - 1;
		private.vector = 0b11; break;
	}
	case 2:
	{
		this->starty = rand() % this->DimY;
		this->startx = 0;
		private.vector = 0b00; break;
	}
	case 3:
	{
		this->starty = rand() % this->DimY;
		this->startx = this->DimX - 1;
		private.vector = 0b10; break;
	}
	} 
	bitSet(this->startx, this->starty);	
	private.flag = this->startx & 1 ? (this->starty & 1)^1 : this->starty & 1;
}

int MazeNext(void* self, unsigned char *nx, unsigned char *ny)
{
	struct Maze* this = self;

	unsigned char x = *nx, y = *ny;
	unsigned char rotct = 0, retcond = 0; // debuggers
	
	// represent a vector rotation
	register unsigned char rotor = rand() % 3;

errhand1:
	private.vector += rotor - 1;
errhand2:// this is the part in which we are taken over direction control manually, but it can work automatically as well

	// Scanning the next position counted by rotor
	switch (private.vector &= 3)
	{
	case 0b00:
		if ((*nx + 1 < this->DimX) && (retcond || !bitTest(*nx + 1, *ny))) { (*nx)++; break; }
		else goto unres;
	case 0b01:
		if ((*ny + 1 < this->DimY) && (retcond || !bitTest(*nx, *ny + 1))) { (*ny)++; break; }
		else goto unres;
	case 0b10:
		if ((*nx - 1 >= 0) && (retcond || !bitTest(*nx - 1, *ny))) { (*nx)--; break; }
		else goto unres;
	case 0b11:
		if ((*ny - 1 >= 0) && (retcond || !bitTest(*nx, *ny - 1))) { (*ny)--; break; }
		else goto unres;
	default:								// unreacheable context used to avoid making function with many arguments
	{
	unres:	
		if (rotct < 3)
		{
			private.vector -= rotor - 1;
			rotor = (rotor + 1) % 3; rotct++;
			goto errhand1;
		}
		else
		{
			register short EResult;
			if ((EResult = private.S->method->bPop(private.S, 2)) == -1)
				return 1;
			else
			{
				private.vector = (unsigned char)(EResult) ^ 2;
				retcond = 1;
				goto errhand2;
			}
		}
	}
	}	// end of switch
	
	if (!retcond)
	{
		private.S->method->bPush(private.S, private.vector, 2);			// if everything's ok push to the stack and return to your business
		bitSet(*nx, *ny);
	}
	if (private.flag)
	{
		this->MazeRep[((y * this->DimX)>>1) + ((x) >> 1)] |=1<< private.vector;
	}
		
	
	private.flag ^= 1;

	return 0;
}

void generateComplete(void* self)
{
	struct Maze* this = self;

	unsigned char x = this->startx;
	unsigned char y = this->starty;

	while (!MazeNext(this, &x, &y));
}

const vftb __method = { MazeNext ,generateComplete };

void* Maze_ctor(void* self, va_list *ap)
{
	struct Maze* this = self;

	this->method = &__method;
	this->DimX = va_arg(*ap, unsigned char);
	this->DimY = va_arg(*ap, unsigned char);
			
	private.SizeInBytes = this->DimX*this->DimY >> 1;
	private.S = new(Stack);
	private.ByteX = this->DimX % 8 > 0 ? this->DimX / 8 + 1 : this->DimX / 8;
	private.matrix = (unsigned char*)malloc(private.ByteX*this->DimY);

	this->MazeRep = malloc(this->DimX*this->DimY>>1);

	for (int i = 0; i < this->DimX*this->DimY >> 1; i++)
		*(this->MazeRep + i) = 0;

	for (int i = 0; i < private.ByteX*this->DimY; i++)
		*(private.matrix + i) = 0;

	srand((unsigned int)time(NULL));
	GetStart(this);
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


