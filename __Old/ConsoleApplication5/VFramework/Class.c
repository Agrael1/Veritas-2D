#include <malloc.h>
#include <assert.h>
#include "Unwinder.h"
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
		__unwinder_suppress();
		rclass->ctor(where, &ap);
		__unwinder_release();
		if(rclass->dtor)
			__stackvar_insert(where, false);
	}
	va_end(ap);
}
void deconstruct(void* self)
{
	const struct Class** cp = self;		
	if (self && *cp && (*cp)->dtor)
	{
		__unwinder_suppress();
		(*cp)->dtor(self);
		__unwinder_release();
		__stackvar_extract(self);
	}	
}

void* _new(const void* type,...)
{
	va_list ap;
	va_start(ap, type);
	void* p = _vnew(type, ap);
	va_end(ap);
	return p;
}
void* _vnew(const void* type, va_list ap)
{
	const struct Class* rclass = type;	// we need to convert pointer from void* to class*
	void* p = calloc(1, rclass->size);	// allocation of memory for class .using size param

	assert(p);							// if Null -> throw an error
	*(const struct Class**)p = rclass;	// passing class descriptor to class (const void* _class)
	if (rclass->ctor)					// if has constructor with some dynal in it, execute with varargs on its input
	{
		__unwinder_suppress();
		rclass->ctor(p, &ap);			// pass arguments as a list of pointers.	
		__unwinder_release();
	}
	return p;
}
void _delete(void* self)
{
	const struct Class** cp = self;		// finds that little bastard (class pointer poiter)
	if (self && *cp && (*cp)->dtor)		// if class has its own destructor the execute before freeing memory that class pointer occupies
	{
		__unwinder_suppress();
		(*cp)->dtor(self);
		__unwinder_release();
	}
	free(self);
}


size_t sizeOf(const void* _self)
{
	const struct Class* const * cp = _self;
	assert(_self&&*cp);
	return (*cp)->size;
}
const char* typeOf_t(const void * _type)
{
	const struct Class* cp = _type;
	assert(_type && (cp)->typestring);
	return (cp)->typestring;
}
const char* typeOf(const void* _self)
{
	const struct Class* const* cp = _self;
	assert(_self && *cp &&(*cp)->typestring);
	return (*cp)->typestring;
}

uintptr_t typeid(const void* _self)
{
	const struct Class* const* cp = _self;
	assert(_self && *cp);
	return (uintptr_t)(*cp);
}

