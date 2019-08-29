// BitStack.c
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Class implementation file
// Stack for small bit values (<8 bit in length)
#include "BitStack.h"
#include "Class.h"

bool virtual(Push)(selfptr, Byte value, Byte length)
{
	account(self);

	if (this->bitctr+length <= NREG)
	{
		private.data[this->head] = ((private.data[this->head]) << length) | value;
		this->bitctr+=length;
		return true;
	}
	else
	{
		if (this->head < DST)
		{
			this->head++;
			this->bitctr = length;
			private.data[this->head] = value;
			return true;
		}
		else return false;	
	}
}
short virtual(Pop)(selfptr, Byte length)
{
	account(self);
	short a;

	if (this->bitctr > 0)
	{
		a = (private.data[this->head]) % (2*length);
		private.data[this->head] >>= length;
		this->bitctr-=length;
		return a;
	}
	else
	{
		if (this->head > 0)
		{
			this->head--;
			this->bitctr = NREG;
			return virtual(Pop)(this, length);
		}
		else
		{
			return -1;
		}
	}
}
void virtual(Reset)(selfptr)
{
	account(self);
	for (int i = 0; i < DST; i++)
		private.data[i] = 0;
}

constructMethodTable(
	.Push = virtual(Push),
	.Pop = virtual(Pop),
	.Reset = virtual(Reset)
);

// Constructor (must be last)
Constructor(void* self, va_list *ap)
{
	account(self);
	assignMethodTable(this);

	this->bitctr = 0;
	this->head = 0;
	
	return this;
}
Destructor(void* self)
{
	return self;
}
ENDCLASSDESC