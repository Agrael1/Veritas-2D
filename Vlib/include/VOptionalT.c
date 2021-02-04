#ifndef OPTIONAL_RESIDE
#pragma pop_macro("OPTIONAL_T")
#endif
#ifdef OPTIONAL_T
#undef c_class
#define c_class Optional(OPTIONAL_T)

extern inline void Destructor(selfptr);
#undef OPTIONAL_T
#include "VOptionalT.c"
#endif