#include "BitSetBase.h"
#include "Class.h"

#define BITSCOUNT (sizeof(size_t)*8)

inline size_t CalcrPos(size_t pos)
{
	return pos / BITSCOUNT;
}
inline size_t CalcdPos(size_t pos)
{
	return (size_t)1 << (pos % BITSCOUNT);
}

bool _IsSet(const selfptr, size_t pos)
{
	if (pos < self->Positions)
		return self->BitArray[CalcrPos(pos)] & CalcdPos(pos);
	return false;
}
bool _Any(const selfptr)
{
	for (size_t i = 0; i < self->Positions; i++)
	{
		if(self->BitArray[CalcrPos(i)] & CalcdPos(i)) return true;
	}
	return false;
}
void _Set(selfptr, size_t pos)
{
	if (pos < self->Positions)
	{
		self->BitArray[CalcrPos(pos)]|=CalcdPos(pos);
	}
}
void _Reset(selfptr, size_t pos)
{
	if (pos <= self->Positions)
	{
		self->BitArray[CalcrPos(pos)] &= ~CalcdPos(pos);
	}
}
void _FullReset(selfptr)
{
	size_t num = (self->Positions % BITSCOUNT) > 0 ? self->Positions / BITSCOUNT + 1 : self->Positions / BITSCOUNT;
	for (size_t i = 0; i < num; i++)
		self->BitArray[i] = 0;
}

VirtualTable{
	.Any = _Any,
	.IsSet = _IsSet,
	.Set = _Set,
	.Reset = _Reset,
	.FullReset = _FullReset
};
Constructor(selfptr, va_list* ap)
{
	assignMethodTable(self);
	self->Positions = va_arg(*ap, unsigned);
	return self;
}
ENDCLASSDESCDD