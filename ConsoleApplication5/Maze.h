#pragma once
#include "BitStack.h"

extern const void* Maze;

#define vftb Maze_vftb

typedef struct {
	int(*MazeNext)(void* self, unsigned char inx, unsigned char iny, unsigned char *nx, unsigned char *ny);
}vftb;

struct Maze
{
	const void* class;
	const vftb* method;
	char _private[32];					// sizeof(struct _private)

	unsigned char DimX, DimY;
	unsigned char startx, starty;
};