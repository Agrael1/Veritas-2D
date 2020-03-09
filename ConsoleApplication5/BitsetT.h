#ifdef N
#include "BitSetBase.h"
#include "Class.h"
#include "Standard.h"
#include "EngineCommons.h"

#define Bitset(x) concat3(Bitset, _, x)
#define c_class Bitset(N)

class
{
	inherits(__BitSetBase);
	size_t BitArray[N / (sizeof(size_t) * 8) + (N % (sizeof(size_t) * 8))];
};

inline Constructor(selfptr, va_list* ap)
{
	construct(self, BitSetBase, N);
	memset32(self->BitArray, 0, sizeof(self->BitArray));
	return self;
}
ENDTEMPLATEDESCDD
#undef c_class
#endif
