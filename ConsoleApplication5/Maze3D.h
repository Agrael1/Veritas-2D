#pragma once
#include "VeritasEngine.h"
#include "Maze.h"
#include "EngineCommons.h"

#define c_class Maze3D

class
{
	inherits(VeritasEngine);

	Byte MazeLength, MazeWidth;
	float fDepth;
	float fFOV;

	struct Frame* MapLayer;
	struct Maze* maze;
};