#ifndef Gamedemo_h
#define Gamedemo_h

#include "olcGameEngine.h"
#include "EngineCommons.h"

#define c_class GameDemo

class
{
	struct olcGameEngine _;

	privatev(
		float m_fPlayerX, m_fPlayerY;
		float m_fPlayerA;
		
		bool key;

		COORD current;
		COORD next;
		);

	float fFOV;

	Byte fDepth;
	Byte cellsX, cellsY;
	Byte CellL;

	struct Frame *localFrame;
	struct Maze* maze;
};


#endif

