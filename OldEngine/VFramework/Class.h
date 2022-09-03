#pragma once
#include <stdarg.h>

struct Class
{
	void (*dtor) (void* self);									// destructor to be able to reverse what c-tor has done
	void (*ctor) (void* self, va_list* app);					// constructor
	const char* typestring;
	size_t size;												// size for construction
};