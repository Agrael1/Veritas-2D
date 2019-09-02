#pragma once
#include "EngineCommons.h"
typedef struct
{
	void* pointer;
	unsigned counter;
}shared_ptr;

#define lea(argtype, pSelf) (argtype*)((pSelf)->pointer)
#define make_shared(type, ...) (shared_ptr){.pointer = new(type, __VA_ARGS__),.counter = 1u}
inline void destroy(shared_ptr* ptr)
{
	if (--ptr->counter == 0)
		if (ptr->pointer) delete(ptr->pointer);
}