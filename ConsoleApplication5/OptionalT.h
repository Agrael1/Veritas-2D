#ifdef T
#include "EngineCommons.h"
#define Optional(x) concat3(Optional, _, x)
#define nullopt(type) concat3(_nullopt,_,Optional(type))
#define c_class Optional(T)

typedef struct c_class
{
	T Value;
	bool bValid;
}c_class;

inline c_class virtual(_nullopt)()
{
	return (c_class) { 0 };
}
#undef c_class
#endif // T
