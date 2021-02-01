#pragma once
typedef struct Bindable_vtbl
{
	void (*Bind)(void* self, struct VLine* gfx);
}Bindable;