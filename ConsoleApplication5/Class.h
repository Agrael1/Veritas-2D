#ifndef Class_h
#define Class_h

#include <stdarg.h>

#define bool _Bool
#define true 1
#define false 0
struct Class
{
	unsigned size;												// size for construction

	void* (*ctor) (void* self, va_list *app);					// constructor, needs pointer on self class and arguments 4 polymorf
	void* (*dtor) (void* self);									// destructor to be able to reverse what c-tor has done
	void* (*clone) (const void* self);							// copy func to copy properly
	int(*differ) (const void* self, const void* b);				// just a simple ass comparator
};



#endif // !Class_h

