#pragma once
#include <VeritasEngine.h>

#pragma push_macro("c_class")
#undef c_class
#define c_class Maze

typedef struct c_class c_class;

struct c_class
{
	VeritasEngine engine;
	uint8_t MazeLength, MazeWidth;
	float fDepth;
	float fFOV;

	bool bShowMap;
	//struct Maze* maze;
};

void Constructor(selfptr, VConsoleDesc screenparams);
void Destructor(selfptr);


int Virtual(Start)(selfptr);


#ifndef MAZE_IMPL
#pragma pop_macro("c_class")
#endif