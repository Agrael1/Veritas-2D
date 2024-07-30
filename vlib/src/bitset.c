#include <vlib/bitset.h>

extern inline bool bitset_at(Bitset_t self, size_t place, size_t limit);
extern inline void bitset_set(Bitset_t self, size_t place, size_t limit);
extern inline void bitset_toggle(Bitset_t self, size_t place, size_t limit);
extern inline void bitset_reset(Bitset_t self, size_t place, size_t limit);
extern inline void bitset_clear(Bitset_t self, size_t limit);