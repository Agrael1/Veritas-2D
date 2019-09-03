#pragma once
#include "EngineCommons.h"
#include "Class.h"
#include <malloc.h>
#include <assert.h>

typedef struct
{
	unsigned counter;
}Control_block;

typedef struct
{
	void* pointer;
	Control_block* control;
}shared_ptr;



#define lea(argtype, pSelf) (argtype*)((pSelf)->pointer)
inline void destroy_shared(shared_ptr* ptr)
{
	if (--ptr->control->counter == 0)
		if (ptr->pointer) delete(ptr->pointer);
}
inline shared_ptr make_shared(void* type, ...)
{
	va_list ap;
	va_start(ap, type);
	shared_ptr s;

	const struct Class* rclass = type;	
	size_t additional = sizeof(Control_block);
	s.pointer = calloc(1, rclass->size + additional);	

	assert(s.pointer);							
	*(const struct Class**)s.pointer = rclass;	
	if (rclass->ctor)					
	{
		s.pointer = rclass->ctor(s.pointer, &ap);
	}
	va_end(ap);
	s.control = (Control_block*)((Byte*)s.pointer + rclass->size);
	s.control->counter = 1u;
	return s;
}