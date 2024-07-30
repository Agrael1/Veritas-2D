#define GAME_IMPL
#include <game/game.h>

bool OnUserCreate(selfptr)
{
    return true;
}

bool OnUserUpdate(selfptr, double fElapsedTime)
{
    ClearFrame(&self->Engine.Swap, (CHAR_INFO){ 'A' });
    return true;
}

bool OnUserDestroy(selfptr)
{
    return true;
}

void Constructor(selfptr, VConsoleDesc desc)
{
    VeritasEngine_ctor(&self->Engine, desc);
    self->Engine.vptr->OnUserCreate = OnUserCreate;
    self->Engine.vptr->OnUserUpdate = OnUserUpdate;
    self->Engine.vptr->OnUserDestroy = OnUserDestroy;
}
void Destructor(selfptr)
{
    VeritasEngine_dtor(&self->Engine);
}



int ArkanoidStart(selfptr)
{
    return Start(&self->Engine);
}