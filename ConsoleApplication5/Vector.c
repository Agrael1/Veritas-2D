#define VECTOR_IMPL
#include "Vector.h"
#include "Class.h"
#include <malloc.h>
#include <assert.h>
#include <memory.h>

Destructor(selfptr);

#define CHUNKSIZE 8u
void* virtual(at)(selfptr, size_t index)
{
	uint8_t* el = self->begin + index * self->elsize;
	if (el > self->end)
		return NULL;
	return el;
}
size_t virtual(count)(const selfptr)
{
	return (self->cur - self->begin) / self->elsize;
}
void* virtual(get_next)(selfptr)
{
	if ((self->cur += self->elsize) >= self->end)
	{
		size_t delta = 2 * (self->end - self->begin);
		size_t delta2 = self->cur - self->begin;
		self->begin = realloc(self->begin, delta);
		self->end = self->begin + delta;
		self->cur = self->begin + delta2;
	}
	return self->cur;
}


VirtualTable{
	.dtor = __rconcat(c_class,_dtor),
	.get_next = virtual(get_next),
	.at = virtual(at),
	.count = virtual(count)
};
Constructor(selfptr, va_list *ap)
{
	InitializeVtable(self);
	self->elsize = va_arg(*ap, size_t);
	self->cur = self->begin = calloc(CHUNKSIZE, self->elsize);
	assert(self->begin);
	self->end = self->begin + self->elsize * CHUNKSIZE;
}
void virtual(construct)(selfptr, size_t elsize, size_t chunksize)
{
	InitializeVtable(self);
	self->elsize = elsize;
	self->cur = self->begin = calloc(chunksize, self->elsize);
	assert(self->begin);
	self->end = self->begin + self->elsize * chunksize;
}
Destructor(selfptr)
{
	free(self->begin);
}
ENDCLASSDESC
