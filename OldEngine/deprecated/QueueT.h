#if defined(T)&&defined(N)&&N<=255
#include "OptionalT.h"
#include "Class.h"
#include "EngineCommons.h"
#define FixedQueue(x,n) concat3(FixedQueue, x, concat3(_,n,_))
#define c_class concat3(FixedQueue, T , concat3(_,N,_))

struct c_class
{
	GENERATED_DESC
	methods(
		Optional(T) (*pop)(selfptr);
		void (*push)(selfptr, T value);
		void (*wipe)(selfptr);
	);
	T container[N];
	unsigned char contains;
	unsigned char current;
};

inline void virtual(push)(selfptr, T value)
{
	if (self->current + 1 < N)
	{
		self->container[self->current++] = value;
	}
	else
	{
		self->container[self->current] = value;
		self->current = 0;
	}
	if (self->contains != N)
		self->contains++;
}
inline Optional(T) virtual(pop)(selfptr)
{
	register unsigned char index = 0;
	if (self->contains > 0)
	{
		if ((short)self->current - self->contains > 0)
		{
			index = self->current - self->contains;
		}
		else
		{
			index = self->current + N - self->contains;
		}
		self->contains--;
		return (Optional(T)){ self->container[index], true };
	}
	return nullopt(T);
}
inline void virtual(wipe)(selfptr)
{
	self->contains = 0;
	self->current = 0;
}

TMethodTable{
	.pop = virtual(pop),
	.push = virtual(push),
	.wipe = virtual(wipe)
};
inline Constructor(selfptr, va_list* ap)
{
	assignMethodTable(self);
	self->contains = 0;
	self->current = 0;
	return self;
}
ENDTEMPLATEDESCDD
#undef c_class
#endif