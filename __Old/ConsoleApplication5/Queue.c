#include "Queue.h"
#include "Class.h"
#include <malloc.h>
#include <string.h>

void virtual(clear)(void* self)
{
	account(self);
	memset(this->insides, 0, this->Capacity*this->Size);
	this->Contains = 0;
	this->cur = 0;
}
bool virtual(empty)(void* self)
{
	account(self);
	return (this->Contains == 0);
}

void virtual(push)(selfptr, const void* smth)
{
	if (self->Contains < self->Capacity)
	{
		memcpy(((Byte*)self->insides + self->cur*self->Size), &smth, self->Size);
		self->cur = (self->cur + 1) % self->Capacity;
		self->Contains++;
	}
}
void virtual(pop)(void* self, void* to)
{
	account(self);
	if (this->Contains)
	{
#pragma warning(push)
		memcpy(to, (Byte*)this->insides + (this->Size*((this->cur - this->Contains) % this->Capacity)) , this->Size);
		this->Contains--;
	}
}

#pragma warning(disable:4113)
constructMethodTable(.push = virtual(push),.pop = virtual(pop),.empty = virtual(empty),.clear = virtual(clear));
#pragma warning(pop)
Constructor(void* self, va_list *ap)
{
	account(self);

	assignMethodTable(this);
	
	this->Size = va_arg(*ap, DWord);
	this->Capacity = va_arg(*ap, DWord);

	this->insides = malloc(this->Size * this->Capacity);
	virtual(clear)(this);

	return this;
}
Destructor(void* self)
{
	account(self);
	free(this->insides);
	return this;
	
}
ENDCLASSDESC