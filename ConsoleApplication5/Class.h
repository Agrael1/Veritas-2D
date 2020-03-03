#pragma once
#include <stdarg.h>

struct Class
{
	size_t size;												// size for construction

	size_t(*ator) (va_list *app);								// custom allocator, uses first args passed to constructor
	void* (*ctor) (void* self, va_list *app);					// constructor
	void* (*dtor) (void* self);									// destructor to be able to reverse what c-tor has done
	const char* typestring;
};