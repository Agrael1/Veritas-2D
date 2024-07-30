// Maze.h
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Class declaration file
// Maze type for my maze generation algorithm
#pragma once
#include "BitStack.h"
#include "EngineCommons.h"

#define c_class Maze

class
{
	const void* _class;

	privatev(
		Byte* matrix;		// Matrix of 0 and 1 for path checking
		struct BitStack* S;	// BitStack for vector saving
		Byte ByteX;			// Parameter for Matrix (Matrix length)
		Byte vector;
		bool flag;			
		);

	methods(
		// Makes the next step in 
		bool(*MazeNext)(void* self, Byte *nx, Byte *ny);
		void(*Generate)(void* self);
	);
// Public:
	Byte* MazeRep;
	Byte DimX, DimY;
	Byte startx, starty;
};