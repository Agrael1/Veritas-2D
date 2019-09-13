#include "Vector.h"
#include "Class.h"
#include <malloc.h>
#include <memory.h>

#define CHUNKSIZE 16u
void* virtual(get)(selfptr, size_t index)
{
	return ((Byte*)self->array + index*self->size);
}
void* virtual(get_next)(selfptr)
{
	if ((++self->elements) > CHUNKSIZE)
		self->array = realloc(self->array, (self->elements + CHUNKSIZE)*self->size);
	return ((Byte*)self->array + self->elements*self->size);
}

VirtualTable{
	.get_next = virtual(get_next),
	.get = virtual(get)
};
Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	self->size = va_arg(*ap, size_t);
	self->elements = -1;
	if (!(self->array = calloc(CHUNKSIZE, self->size)))
		return nullptr;
	return self;
}
void* virtual(construct)(selfptr, size_t elsize, size_t chunksize)
{
	assignMethodTable(self);
	self->size = elsize;
	self->elements = -1;
	if (!(self->array = calloc(chunksize, self->size)))
		return nullptr;
	return self;
}
Destructor(selfptr)
{
	free(self->array);
	return self;
}
ENDCLASSDESC
