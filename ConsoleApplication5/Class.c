#include <malloc.h>
#include <assert.h>
#include "Class.h"
#include "New.h"

void construct(void* where, const void* what, ...)
{
	va_list ap;
	va_start(ap, what);
	const struct Class* rclass = what;

	assert(where);
	*(const struct Class**)where = rclass;
	if (rclass->ctor)
	{
		rclass->ctor(where, &ap);
	}
	va_end(ap);
}
void deconstruct(void* self)
{
	const struct Class** cp = self;		
	if (self&&*cp && (*cp)->dtor)		
		self = (*cp)->dtor(self);
}

void* new(const void* _class,...){
	va_list ap;
	va_start(ap, _class);
	const struct Class* rclass = _class;	// we need to convert pointer from void* to class*
	size_t additional = 0;
	if (rclass->ator)
	{
		additional = rclass->ator(&ap);
	}
	void *p = calloc(1, rclass->size + additional);	// allocation of memory for class .using size param
	
	assert(p);							// if Null -> throw an error
	*(const struct Class**)p = rclass;	// passing class descriptor to class (const void* _class)
	if (rclass->ctor)					// if has constructor with some dynal in it, execute with varargs on its input
	{
		p = rclass->ctor(p, &ap);		// pass arguments as a list of pointers.	
	}
	va_end(ap);
	return p;							//returns a pointer to class pointer
}
void delete(void* self)
{
	const struct Class** cp = self;		// finds that little bastard (class pointer poiter)
	if (self&&*cp && (*cp)->dtor)		// if class has its own destructor the execute before freeing memory that class pointer occupies
		self = (*cp)->dtor(self);
	free(self);
}

unsigned int sizeOf(const void* _self)
{
	const struct Class* const * cp = _self;
	assert(_self&&*cp);
	return (*cp)->size;
}
const char* stringOf(const void * _self)
{
	const struct Class* cp = _self;
	assert(_self && (cp)->typestring);
	return (cp)->typestring;
}

