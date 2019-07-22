#include "BitField.h"
#include <malloc.h>
#include "Class.h"

#define BITSCOUNT (sizeof(MaxInt)*8)

inline size_t CalcrPos(DWord pos) 
{
	return pos / BITSCOUNT;
}
inline size_t CalcdPos(DWord pos)
{
	return 1i64 << (pos % BITSCOUNT);
}

bool _IsSet(selfptr, DWord pos)
{
	if (pos < self->Positions)
		return self->BitArray[CalcrPos(pos)] & CalcdPos(pos);
	return false;
}
bool _Any(selfptr)
{
	for (DWord i = 0; i < self->Positions; i++)
	{
		if(self->BitArray[CalcrPos(i)] & CalcdPos(i)) return true;
	}
	return false;
}
bool _Set(selfptr, DWord pos)
{
	if (pos < self->Positions)
	{
		self->BitArray[CalcrPos(pos)]|=CalcdPos(pos);
		return true;
	}
	return false;
}
bool _Reset(selfptr, DWord pos)
{
	if (pos <= self->Positions)
	{
		self->BitArray[CalcrPos(pos)] &= ~CalcdPos(pos);
		return true;
	}
	return false;
}
bool _FullReset(selfptr)
{
	if (self->Positions > 0)
	{
		size_t num = (self->Positions % BITSCOUNT) > 0 ? self->Positions / BITSCOUNT + 1 : self->Positions / BITSCOUNT;
		for (size_t i = 0; i < num; i++)
			self->BitArray[i] = 0;
		return true;
	}
	return false;
}

constructMethodTable(
	.Any = _Any,
	.IsSet = _IsSet,
	.Set = _Set,
	.Reset = _Reset,
	.FullReset = _FullReset
);

Constructor(selfptr, va_list* ap)
{
	assignMethodTable(self);
	self->Positions = va_arg(*ap, DWord);
	return self;
}
Destructor(selfptr)
{
	return self;
}
Allocator(va_list* ap)
{
	size_t positions = va_arg(*ap, DWord);
	size_t num = (positions % BITSCOUNT) > 0 ?
		positions / BITSCOUNT + 1 :
		positions / BITSCOUNT;
	return num * sizeof(MaxInt);
}
ENDCLASSDESCWA