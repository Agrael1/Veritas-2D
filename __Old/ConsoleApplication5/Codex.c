#include "Codex.h"

void DestroyCodex()
{
	deconstruct(&GetCodex()->binds);
}

extern struct Class _shared_ptr;

Codex* GetCodex()
{
	static Codex codex;
	if(!codex.binds._class)
		construct(&codex.binds, HashMap, &_shared_ptr);
	return &codex;
}
