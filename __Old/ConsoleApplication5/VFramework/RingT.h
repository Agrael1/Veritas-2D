/********************************************************************************/
/*Not a class variation of Ring, usage with classes will result in memory leaks */
/********************************************************************************/

#if defined(T)&&defined(N)&&N<=255
#include <OptionalT.h>
#include <Class.h>
#include <VDefs.h>

#define Ring(x,n) concat4(c,Ring, x, concat3(_,n,_))
#define RingT(x,n) concat3(Ring, x, concat3(_,n,_))
#define c_class RingT(T,N)

struct __rconcat(c_class, _base)
{
	T container[N];
	unsigned char contains;
	unsigned char current;
};
interface
{
	Optional(T) (*pop)(selfptr);
	void (*push)(selfptr, T value);
	void (*wipe)(selfptr);
};
ComposeTypeEx;

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
	InitializeVtableEx(self);
	self->contains = 0;
	self->current = 0;
}
ENDTEMPLATEDESCDD
#undef c_class
#endif