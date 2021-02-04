#pragma once
#include <limits.h>
#include <stdbool.h>

#define Bitset(N, Name) size_t Name[((N)/(sizeof(size_t)*CHAR_BIT))+!!((N)%(sizeof(size_t)*CHAR_BIT))]
typedef size_t Bitset_t[];

/// @brief Returns a bit value at [place]
/// @param self - bitset itself
/// @param place - where to search
/// @param limit - bitset limit
/// @return value of bit
inline bool bitset_at(const Bitset_t self, size_t place, size_t limit)
{
	return place > limit ? false : !!((1ull << place % (sizeof(size_t) * CHAR_BIT)) & self[place / (sizeof(size_t) * CHAR_BIT)]);
}

/// @brief Sets a bit value at [place]
/// @param self  - bitset itself
/// @param place - where to change
/// @param limit - bitset limit
inline void bitset_set(Bitset_t self, size_t place, size_t limit)
{
	if (place < limit)	self[place / (sizeof(size_t) * CHAR_BIT)] |= 1ull << place % (sizeof(size_t) * CHAR_BIT);
}

/// @brief toggles a bit value at [place]
/// @param self  - bitset itself
/// @param place - where to change
/// @param limit - bitset limit
inline void bitset_toggle(Bitset_t self, size_t place, size_t limit)
{
	if (place < limit)	self[place / (sizeof(size_t) * CHAR_BIT)] ^= 1ull << place % (sizeof(size_t) * CHAR_BIT);
}

/// @brief Resets a bit value at [place]
/// @param self  - bitset itself
/// @param place - where to change
/// @param limit - bitset limit 
inline void bitset_reset(Bitset_t self, size_t place, size_t limit)
{
	if (place < limit)	self[place / (sizeof(size_t) * CHAR_BIT)] &= ~(1ull << place % (sizeof(size_t) * CHAR_BIT));
}

/// @brief Clears all the values
/// @param self  - bitset itself
/// @param limit - bitset limit 
inline void bitset_clear(Bitset_t self, size_t limit)
{
	for (size_t i = 0; i < ((limit) / (sizeof(size_t) * CHAR_BIT)) + !!((limit) % (sizeof(size_t) * CHAR_BIT)); i++)
	{
		self[i] = 0;
	}
}