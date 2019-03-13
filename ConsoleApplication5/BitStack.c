#include "BitStack.h"
#include "Class.h"
#include <setjmp.h>
#include <stdio.h>

#if _WIN64
#define NREG 64
#define DST 30
#else
#define NREG 32
#define DST 50
#endif

void bPush(void* self, unsigned char value, unsigned char length)
{
	struct Stack* this = self;

	if (this->bitctr < 32)
	{
		this->data[this->head] = ((this->data[this->head]) << 2) | value;
		//printf("pushed => %d\n", value);
		this->bitctr++;
	}
	else
	{
		if (this->head < 50)
		{
			//printf("pushed => %I64u to big stack\n", this->data[this->head]);
			this->head++;

			this->bitctr = 1;
			this->data[this->head] = value;
		}
		//else
			//printf("Stack overflow!");
	}
}

short bPop(void* self, unsigned char length)
{
	struct Stack* this = self;
	short a;
	if (this->bitctr > 0)
	{
		a = (this->data[this->head]) % 4;
		//printf("popped <= %d\n", a);
		this->data[this->head] >>= 2;
		this->bitctr--;
		return a;
	}
	else
	{
		if (this->head > 0)
		{
			//printf("popped <= %I64u from the big stack\n", this->data[this->head]);
			this->head--;
			this->bitctr = 32;

			bPop(this, length);
		}
		else
		{
			return -1;
		}
	}
}

// Constructor (must be last)
void* stack_ctor(void* self, va_list *ap)
{
	struct Stack* this = self;

	this->bitctr = 1;
	this->bPop = bPop;
	this->bPush = bPush;
	this->head = 0;

	return this;
}

const struct Class _stack = {.size = sizeof(struct Stack),.ctor = stack_ctor};
const void* Stack = &_stack;