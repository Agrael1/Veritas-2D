#pragma once
#include "BitStack.h"

#pragma region Bindings
#define vftb Maze_vftb
typedef struct {
	int(*MazeNext)(void* self, unsigned char inx, unsigned char iny, unsigned char *nx, unsigned char *ny);
}vftb;
typedef struct 
{
	unsigned char* matrix;
	struct Stack* S;
	unsigned char ByteX;
	unsigned char SizeInBytes;
	unsigned char vector;
	_Bool flag;
}Maze_private;
#pragma endregion

extern const void* Maze;

struct Maze
{
//Private:
	const void* class;
	char __[sizeof (Maze_private)];
// Public:
	const vftb* method;
	unsigned char* MazeRep;
	unsigned char DimX, DimY;
	unsigned char startx, starty;
};