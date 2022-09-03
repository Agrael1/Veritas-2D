#pragma once
#include <VeritasEngine.h>
#include <Generator.h>

#pragma push_macro("c_class")
#undef c_class
#define c_class Maze

typedef struct c_class c_class;

struct c_class
{
	VeritasEngine engine;
	float fDepth;
	float fFOV;

	bool bShowMap;

	Generator maze;
	uint32_t step;
	//struct Maze* maze;
};

void Constructor(selfptr, VConsoleDesc screenparams, uint16_t maze_width, uint16_t maze_height);
void Destructor(selfptr);


int Virtual(Start)(selfptr);


#ifndef MAZE_IMPL
#pragma pop_macro("c_class")
#endif