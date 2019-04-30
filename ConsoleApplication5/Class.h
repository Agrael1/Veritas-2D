#ifndef Class_h
#define Class_h

#include <stdarg.h>

struct Class
{
	
	unsigned size;												// size for construction

	void* (*ctor) (void* self, va_list *app);					// constructor, needs pointer on self class and arguments 4 polymorf
	void* (*dtor) (void* self);									// destructor to be able to reverse what c-tor has done
	const char* typestring;
};



#endif // !Class_h

