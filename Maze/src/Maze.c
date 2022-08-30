#define MAZE_IMPL
#include <Maze.h>


bool OnUserCreate(selfptr)
{
	return true;
}
bool OnUserUpdate(selfptr, double fElapsedSeconds)
{
	return true;
}
bool OnUserDestroy(selfptr)
{
	return true;
}




void Constructor(selfptr, VConsoleDesc screenparams)
{
	VeritasEngine_ctor(&self->engine, screenparams);
	self->engine.method->OnUserCreate = OnUserCreate;
	self->engine.method->OnUserUpdate = OnUserUpdate;
	self->engine.method->OnUserDestroy = OnUserDestroy;
}
void Destructor(selfptr)
{
	VeritasEngine_dtor(&self->engine);
}




int Virtual(Start)(selfptr)
{
	Start(&self->engine);
}