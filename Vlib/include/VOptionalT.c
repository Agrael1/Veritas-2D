#pragma pop_macro("OPTIONAL_T")
#ifdef OPTIONAL_T
#undef c_class
#define c_class Optional(OPTIONAL_T)

extern inline void Destructor(selfptr);
#endif