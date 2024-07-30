// BitStack.c
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Class implementation file
// Stack for small bit values (<8 bit in length)
#include "BitStack.h"
#include "Class.h"

bool virtual(push)(selfptr, uint8_t value, uint8_t length)
{
	if (self->bitctr+length <= sizeof(size_t))
	{
		self->data[self->head] = ((self->data[self->head]) << length) | value;
		self->bitctr+=length;
		return true;
	}
	else
	{
		if (self->head < DST)
		{
			self->head++;
			self->bitctr = length;
			self->data[self->head] = value;
			return true;
		}
		else return false;	
	}
}
short virtual(pop)(selfptr, uint8_t length)
{
	short a;

	if (self->bitctr > 0)
	{
		a = (self->data[self->head]) % (2*length);
		self->data[self->head] >>= length;
		self->bitctr-=length;
		return a;
	}
	else
	{
		if (self->head > 0)
		{
			self->head--;
			self->bitctr = sizeof(size_t);
			return virtual(pop)(self, length);
		}
		else
		{
			return -1;
		}
	}
}
void virtual(reset)(selfptr)
{
	for (int i = 0; i < DST; i++)
		self->data[i] = 0;
}

VirtualTable
{
	.push = virtual(push),
	.pop = virtual(pop),
	.reset = virtual(reset)
};
Constructor(selfptr, va_list *ap)
{
	InitializeVtable(self);
	self->bitctr = 0;
	self->head = 0;
}
ENDCLASSDESCDD