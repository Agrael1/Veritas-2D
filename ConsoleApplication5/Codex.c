#include "Codex.h"
#include "SharedPtr.h"
Codex codex;

void InitializeCodex()
{
	construct(&codex.binds, HashMap, sizeof(shared_ptr));
}
void DestroyCodex()
{
	deconstruct(&codex.binds);
}