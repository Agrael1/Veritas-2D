// Maze.h
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Class declaration file
// Maze type for my maze generation algorithm
#pragma once
#include "BitStack.h"

#pragma region Bindings

#undef c_class
#define c_class Maze

typedef struct {
	int(*MazeNext)(void* self, unsigned char *nx, unsigned char *ny);
	void(*generateComplete)(void* self);
}vftb;
#pragma endregion

extern const void* Maze;

struct Maze
{
	const void* _class;
	privatev(
		Byte* matrix;		// Matrix of 0 and 1 for path checking
		struct Stack* S;	// BitStack for vector saving
		Byte ByteX;			// Parameter for Matrix (Matrix length)
		Byte vector;
		_Bool flag;
		);

// Public:
	const vftb* method;
	Byte* MazeRep;
	Byte DimX, DimY;
	Byte startx, starty;
};