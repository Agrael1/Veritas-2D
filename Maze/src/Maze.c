#define MAZE_IMPL
#include <Maze.h>

enum DEF_COLOUR
{
	DFG_BLACK = 0x0000,
	DFG_DARK_BLUE = 0x0001,
	DFG_DARK_GREEN = 0x0002,
	DFG_DARK_CYAN = 0x0003,
	DFG_DARK_RED = 0x0004,
	DFG_DARK_MAGENTA = 0x0005,
	DFG_DARK_YELLOW = 0x0006,
	DFG_GREY = 0x0007,
	DFG_DARK_GREY = 0x0008,
	DFG_BLUE = 0x0009,
	DFG_GREEN = 0x000A,
	DFG_CYAN = 0x000B,
	DFG_RED = 0x000C,
	DFG_MAGENTA = 0x000D,
	DFG_YELLOW = 0x000E,
	DFG_WHITE = 0x000F,

	DBG_BLACK = 0x0000,
	DBG_DARK_BLUE = 0x0010,
	DBG_DARK_GREEN = 0x0020,
	DBG_DARK_CYAN = 0x0030,
	DBG_DARK_RED = 0x0040,
	DBG_DARK_MAGENTA = 0x0050,
	DBG_DARK_YELLOW = 0x0060,
	DBG_GREY = 0x0070,
	DBG_DARK_GREY = 0x0080,
	DBG_BLUE = 0x0090,
	DBG_GREEN = 0x00A0,
	DBG_CYAN = 0x00B0,
	DBG_RED = 0x00C0,
	DBG_MAGENTA = 0x00D0,
	DBG_YELLOW = 0x00E0,
	DBG_WHITE = 0x00F0,
};

// False makes application quit
static bool HandleInputEvents(void* self, const KeyboardEvent event)
{
	if (event.type == Press)
		switch (event.code)
		{
		case VK_ESCAPE:
			return false;
		}
	return true;
}

bool OnUserCreate(selfptr)
{
	Subdivide(&self->maze, 200, 70, 70);
	MakeRooms(&self->maze, 20, 30);
	return true;
}
bool OnUserUpdate(selfptr, double fElapsedSeconds)
{
	BSPNode* x = self->maze.graph;
	while (x != self->maze._end)
	{
		if(x->left == NULL && x->right == NULL)
			DrawRectangleWireframe(&self->engine.Swap, x->x1, x->y1, x->x2, x->y2, (CHAR_INFO) { L' ', DBG_DARK_RED });
		x++;
	}
	return true;
}
bool OnUserDestroy(selfptr)
{
	Generator_dtor(&self->maze);
	return true;
}




void Constructor(selfptr, VConsoleDesc screenparams, uint16_t maze_width, uint16_t maze_height)
{
	VeritasEngine_ctor(&self->engine, screenparams);
	self->engine.method->OnUserCreate = OnUserCreate;
	self->engine.method->OnUserUpdate = OnUserUpdate;
	self->engine.method->OnUserDestroy = OnUserDestroy;
	self->engine.method->HandleInputEvents = HandleInputEvents;
	self->maze.width = maze_width;
	self->maze.height = maze_height;
	self->step = 0;
}
void Destructor(selfptr)
{
	VeritasEngine_dtor(&self->engine);
}




int Virtual(Start)(selfptr)
{
	Start(&self->engine);
}