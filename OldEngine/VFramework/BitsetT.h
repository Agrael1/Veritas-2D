#ifdef N
#include "BitSetBase.h"
#include <Class.h>
#include "Standard.h"
#include <VDefs.h>

#define Bitset(x) concat4(c,Bitset, _, x)
#define BitsetT(x) concat3(Bitset, _, x)
#define c_class BitsetT(N)

typedef struct c_class
{
	extends(__BitSetBase);
	size_t BitArray[N / (sizeof(size_t) * 8) + (N % (sizeof(size_t) * 8))];
}Bitset(N);

inline Constructor(selfptr, va_list* ap)
{
	construct(self, BitSetBase, N);
	memset32(self->BitArray, 0, sizeof(self->BitArray));
}
ENDTEMPLATEDESCDD
#undef c_class
#endif
