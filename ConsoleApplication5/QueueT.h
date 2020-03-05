#if defined(T)&&defined(N)
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
	size_t contains;
	size_t current;
};

inline void virtual(push)(selfptr, T value)
{
	if (self->contains < N)
	{
		self->contains++;
		self->container[self->current++] = value;
	}
	else
	{
		self->container[self->current] = value;
		self->current = self->current == N ? 0 : self->current + 1;
	}
}
inline Optional(T) virtual(pop)(selfptr)
{
	if (self->contains > 0)
	{
		self->contains--;
		return (Optional(T)){ self->container[--self->current], true };
	}
	return nullopt(T)();
}
inline void virtual(wipe)(selfptr)
{
	self->contains = 0;
	self->current = 0;
}

VirtualTable{
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
inline Destructor(void* self)
{
	return self;
}
ENDCLASSDESC
#undef c_class
#endif