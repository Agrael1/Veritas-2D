#include "EventQueue.h"
#include "Class.h"
#include <malloc.h>
#include <memory.h>

void* _Read(selfptr)
{
	if (self->Contains > 0)
		return (Byte*)self->Container +
		((self->cur - self->Contains--) % self->Capacity)*
		self->ElementSize;
	return nullptr;
}
void* _GetNext(selfptr)
{
	void* _proxy = (Byte*)self->Container +
		self->cur*self->ElementSize;
	self->cur = (++self->cur) % self->Capacity;

	// if element is higher then capacity, the memory, 
	// provided by func is overriding existing
	self->Contains + 1 > self->Capacity ?
		self->Capacity : self->Contains++;
	return _proxy;
}
void _Wipe(selfptr)
{
	self->Contains = 0;
	self->cur = 0;
}

constructMethodTable(
	.Read = _Read,
	.GetNext = _GetNext,
	.Wipe = _Wipe
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	self->ElementSize = va_arg(*ap, DWord);
	self->Capacity = va_arg(*ap, DWord);
	self->Container = _aligned_malloc(self->ElementSize*self->Capacity, va_arg(*ap, DWord));
	self->Contains = self->cur = 0;
	return self;
}
Destructor(selfptr)
{
	_aligned_free(self->Container);
	return self;
}
ENDCLASSDESC