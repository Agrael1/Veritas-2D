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
	struct Stack* this = self;

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
	struct Stack* this = self;
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

const vftb method = { bPush,bPop };

// Constructor (must be last)
void* stack_ctor(void* self, va_list *ap)
{
	struct Stack* this = self;

	this->bitctr = 0;
	this->head = 0;
	this->method = &method;

	return this;
}

const struct Class _stack = {.size = sizeof(struct Stack),.ctor = stack_ctor};
const void* Stack = &_stack;