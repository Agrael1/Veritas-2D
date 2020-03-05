#define N 16
#ifdef N
#include "EngineCommons.h"

#define Bitset(x) concat3(Bitset, _, x)
#define c_class Bitset(N)

class
{
	GENERATED_DESC
	methods(
		bool(*Any)(selfptr);
		bool(*IsSet)(selfptr, DWord pos);
		bool(*Set)(selfptr, DWord pos);
		bool(*Reset)(selfptr, DWord pos);
		bool(*FullReset)(selfptr);
	);

	size_t Positions;
	size_t BitArray[N/sizeof(size_t)+(N%sizeof(size_t)>0)];
};
#endif
