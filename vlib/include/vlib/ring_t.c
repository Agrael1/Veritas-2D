#pragma pop_macro("RING_T")
#ifdef RING_T
#undef c_class
#define c_class Ring(RING_T)

extern inline RING_T* Template(top)(selfptr);
extern inline void Template(clear)(selfptr);
extern inline NODISCARD RING_T* Template(emplace)(selfptr);
extern inline void Destructor(selfptr);
extern inline void Constructor(selfptr, uint16_t length);

#undef RING_T
#include "ring.c"
#endif