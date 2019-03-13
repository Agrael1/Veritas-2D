#pragma once
#include "BitStack.h"

extern const void* Maze;

struct Maze 
{
	const void* class;

	unsigned char DimX, DimY;
	unsigned char startx, starty;
	// todo comment
	int (*MazeNext)(void* self, unsigned char inx, unsigned char iny, unsigned char *nx, unsigned char *ny);
};