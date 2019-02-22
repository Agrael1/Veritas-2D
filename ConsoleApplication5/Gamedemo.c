#include "GameDemo.h"
#include "Class.h"

static const struct Class _GameDemo = 
{
	sizeof(struct GameDemo), 
	GameDemo_ctor
};

const void* GameDemo = &_GameDemo;