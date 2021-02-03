#pragma pop_macro("RING_T")
#ifdef RING_T
#undef c_class
#define c_class Ring(RING_T)

extern inline void Template(_Destroy_single)(RING_T* element);
extern inline RING_T* Template(top)(selfptr);
extern inline void Template(clear)(selfptr);
extern inline void Destructor(selfptr);


void Constructor(selfptr, uint16_t length)
{
	self->contains = self->current = 0;
	self->limit = length;
	ALLOC_CHECK(self->container = calloc(length, sizeof(RING_T)));
}

RING_T* Template(emplace)(selfptr)
{
	if (self->contains == self->limit)
	{
		Template(_Destroy_single)(&self->container[self->current]);
	}
	RING_T* out = &self->container[self->current];
	self->current = (self->current + 1) % self->limit;
	if (self->contains < self->limit)self->contains++;
	return out;
}


#undef RING_T
//#include "VRingT.c"
#endif