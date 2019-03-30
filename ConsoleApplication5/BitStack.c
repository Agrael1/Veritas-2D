// BitStack.c
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Class implementation file
// Stack for small bit values (<8 bit in length)
#include "BitStack.h"
#include "Class.h"

int bPush(void* self, Byte value, Byte length)
{
	struct c_class* this = self;

	if (this->bitctr+length <= NREG)
	{
		private.data[this->head] = ((private.data[this->head]) << length) | value;
		this->bitctr+=length;
		return 0;
	}
	else
	{
		if (this->head < DST)
		{
			this->head++;
			this->bitctr = length;
			private.data[this->head] = value;
			return 0;
		}
		else return -1;	
	}
}

short bPop(void* self, Byte length)
{
	struct c_class* this = self;
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
			bPop(this, length);
		}
		else
		{
			return -1;
		}
	}
}

constructMethodTable(bPush,.bPop = bPop);

// Constructor (must be last)
void* stack_ctor(void* self, va_list *ap)
{
	struct c_class* this = self;
	assignMethodTable(this);

	this->bitctr = 0;
	this->head = 0;
	
	return this;
}

const struct Class _stack = {.size = sizeof(struct c_class),.ctor = stack_ctor};
const void* c_class = &_stack;