#ifndef Gamedemo_h
#define Gamedemo_h

#include "olcGameEngine.h"

extern const void* GameDemo;

struct GameDemo
{
	struct olcGameEngine _;
	struct Maze* maze;
};


#endif

