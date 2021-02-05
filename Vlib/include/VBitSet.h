// Copyright (c) 2020
 //      Ilya Doroshenko, xdoros01
 //      agrael@live.ru, xdoros01@fit.vutbr.cz
 // All rights reserved.
 //
 // This code is licensed under the MIT License.
 //
 // Permission is hereby granted, free of charge, to any person obtaining a copy
 // of this software and associated documentation files(the "Software"), to deal
 // in the Software without restriction, including without limitation the rights
 // to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 // copies of the Software, and to permit persons to whom the Software is
 // furnished to do so, subject to the following conditions :
 //
 // The above copyright notice and this permission notice shall be included in
 // all copies or substantial portions of the Software.
 //
 // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 // THE SOFTWARE.

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