#ifndef Class_h
#define Class_h

#include <stdarg.h>

struct Class
{
	unsigned size;												// size for construction

	size_t(*ator) (va_list *app);								// custom allocator, uses first args passed to constructor
	void* (*ctor) (void* self, va_list *app);					// constructor
	void* (*dtor) (void* self);									// destructor to be able to reverse what c-tor has done
	const char* typestring;
};
#endif // !Class_h

