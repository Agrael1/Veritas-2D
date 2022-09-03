#ifdef T
#include <VDefs.h>
#define Optional(x) concat3(Optional, _, x)
#define nullopt(type) ((Optional(type)){0})
#define c_class Optional(T)

typedef struct c_class
{
	T Value;
	bool bValid;
}c_class;
#undef c_class
#endif // T
