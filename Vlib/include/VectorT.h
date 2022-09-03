// Copyright (c) 2020
//		Ilya Doroshenko, xdoros01
//		agrael@live.ru, xdoros01@fit.vutbr.cz
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

/**
 * @file VectorT.h
 * @author Ilya Doroshenko
 * @brief Vector template header
 */

#ifdef VECTOR_T
#pragma push_macro("c_class")

#include "RuntimeClass.h"
#include <stdbool.h>

#define Vector(T__) __rcat2(Vector_,T__)
#define unique_vector(T__) UNIQUE(Vector(T__))

#undef c_class
#define c_class Vector(VECTOR_T)
#pragma push_macro("VECTOR_T")

typedef struct c_class c_class;

/// @brief Constructs a new vector with capacity for 32 elements
/// @param  [inout]self -> pointer to vector
void Constructor(selfptr);

/// @brief destroys the vector, releasing all of its elements if necessary
/// @param  [in]self -> pointer to valid vector
void Destructor(selfptr);

#ifndef TC
/// @brief Do NOT use contents directly
struct c_class
{
	VECTOR_T* data_;        /**< points to first element*/
	VECTOR_T* end_;         /**< points to one past last element*/
	size_t capacity_;		/**< internal storage*/
};
#ifdef IsClass
extern void __rdtor(VECTOR_T)(VECTOR_T* self);

inline void Template(_Destroy)(VECTOR_T* _First, VECTOR_T* _Last)
{
	while (_First != _Last)
	{
		__rdtor(VECTOR_T)(_First++);
	}
}
inline void Template(_Destroy_single)(VECTOR_T* element)
{
	__rdtor(VECTOR_T)(element);
}
#else
inline void Template(_Destroy)(VECTOR_T* a, VECTOR_T* b)
{
}
inline void Template(_Destroy_single)(VECTOR_T* a)
{
}
#endif // IsClass
#endif

/// @brief Move constructor, moves content from one vector to another
/// leaving first empty
/// @param [in]self -> pointer to valid vector
/// @param [in]other  vector to move from
inline void __rcat2(c_class, _move_ctor)(selfptr, c_class* other)
{
	*self = *other;
	other->data_ = other->end_ = NULL;
	other->capacity_ = 0;
	Constructor(other);
}

/// @brief 
/// @param [in]self -> pointer to valid vector
/// @param [in]n  position
/// @return reference to an element at certain position
NODISCARD VECTOR_T* Template(at)(const selfptr, size_t n);


/// @brief 
/// @param [in]self -> pointer to valid vector
/// @return current vector size
NODISCARD size_t Template(size)(const selfptr);

/// @brief Checks wether the vector is empty
/// @param [in]self -> pointer to valid vector
/// @return 
NODISCARD bool Template(empty)(const selfptr);

/// @brief Get reference to the first element
/// @param  [in]self -> pointer to valid vector
/// @return 
NODISCARD VECTOR_T* Template(front)(const selfptr);

/// @brief Get reference to the last element
/// @param  [in]self -> pointer to valid vector
/// @return 
NODISCARD VECTOR_T* Template(back)(const selfptr);

/// @brief Reserves new capacity
/// @param  [inout]self -> pointer to valid vector
/// @param  [in]new_cap  capacity in elements
void Template(reserve)(selfptr, size_t new_cap);

/// @brief Shrinks allocation to fit elemnts
/// @param  [inout]self -> pointer to valid vector
void Template(shrink_to_fit)(selfptr);

/// @brief deconstructs all the elements in vector
/// @param  [inout]self -> pointer to valid vector
void Template(clear)(selfptr);

/// @brief Erases single element shifting the array, 
/// usage: erase_Vector_T(back_Vector_T(&v)-3)
/// @param  [inout]self -> pointer to valid vector
/// @param  [in]iterator -> pointer to an element
void Template(erase)(selfptr, VECTOR_T* iterator);

/// @brief Erases multiple elements [_Start;_End) shifting the array, 
/// usage: erase_Vector_T(front_Vector_T(&v), back_Vector_T(&v)-3)
/// @param  [inout]self -> pointer to valid vector
/// @param  [in]_Start -> pointer to first element to erase
/// @param  [in]_End -> pointer after Last element 
void Template(erase_range)(selfptr, VECTOR_T* _Start, VECTOR_T* _End);

/// @brief Pushes element in vector like in stack
/// Emplace semantics - returns space for the new element to be constructed on
/// @param [inout]self -> pointer to valid vector
VECTOR_T* Template(push_back)(selfptr);

/// @brief Pops element from the vector like from stack
/// @param [inout]self -> pointer to valid vector
void Template(pop_back)(selfptr);

/// @brief Pops element from the vector like from queue
/// @param [inout]self -> pointer to valid vector
inline void Template(pop_front)(selfptr)
{
	Template(erase)(self, self->data_);
}

#undef IsClass
#undef VECTOR_T
#undef c_class

#ifndef VECTOR_IMPL
#pragma pop_macro("c_class")
#endif // !VECTOR_IMPL
#endif