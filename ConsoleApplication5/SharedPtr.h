#pragma once
typedef struct
{
	void* pointer;
	unsigned counter;
}shared_ptr;

#define lea(argtype, pSelf) (argtype*)((pSelf)->pointer)
#define make_shared(type, ...) (shared_ptr){.pointer = new(type, __VA_ARGS__),.counter = 1u}