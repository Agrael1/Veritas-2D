#pragma once
#include "VDefs.h"

#define c_class Vector

struct Vector;

extern const void* c_class; 

struct c_class
{
	interface
	{
		void (*dtor)(selfptr);
		void*(*get_next)(selfptr);
		void*(*at)(selfptr, size_t index);
		size_t(*count)(const selfptr);
	} *method;
	uint8_t* begin;
	uint8_t* end;
	uint8_t* cur;
	size_t elsize;
};

void virtual(construct)(selfptr, size_t elsize, size_t chunksize);

#ifndef VECTOR_IMPL
#undef c_class
#endif // !VECTOR_IMPL