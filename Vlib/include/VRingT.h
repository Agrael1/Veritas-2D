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


#ifdef RING_T
#include <malloc.h>
#include "RuntimeClass.h"

#undef c_class
#define c_class Ring(RING_T)
#pragma push_macro("RING_T")

#define Ring(T__) __rcat2(Ring_,T__)
#define unique_ring(T__) UNIQUE(Ring(T__))

typedef struct c_class c_class;

/// @brief Constructs a new ring
/// @param length - length of ring
void Constructor(selfptr, uint16_t length);

/// @brief Destroys all the elements in ring as well as the ring itself
void Destructor(selfptr);

#ifndef TC
struct c_class
{
	uint16_t contains; ///<How many elements are currently held inside ring, cannot be greater than limit
	uint16_t current; ///<Current position in the queue
	uint16_t limit;		///<Maximum amount of placed elements
	RING_T* container;	///<Container for elements
};
#endif //TC

/// @brief Emplaces value into ring, providing pointer to a place of construction
/// @return valid pointer to an object to be initialized
NODISCARD RING_T* Template(emplace)(selfptr);

/// @brief Provides pointer to a topmost value
/// @return valid pointer to an object, if ring is empty then NULL
inline NODISCARD RING_T* Template(top)(selfptr)
{
	return self->contains ? &self->container[(self->current - 1) % self->limit] : NULL;
}

/// @brief Destroys top value
inline void Template(pop)(selfptr)
{
	if (!self->contains) return;
	self->current = (self->current - 1) % self->limit;
	self->contains--;
#ifdef IsClass
	__rdtor(RING_T)(&self->container[self->current]);
#endif // IsClass
}
/// @brief Clears entire ring
inline void Template(clear)(selfptr)
{
	while (self->contains)Template(pop)(self);
	self->current = 0;
}

inline void Destructor(selfptr)
{
#ifdef IsClass
	Template(clear)(self);
#endif // IsClass
	free(self->container);
}

#undef IsClass
#undef RING_T
#endif // RING_T