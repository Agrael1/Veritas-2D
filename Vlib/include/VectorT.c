/**
 * @file Vector.c
 * @author Ilya Doroshenko
 * @brief Vector template implementation
 */
#pragma pop_macro("VECTOR_T")
#ifdef VECTOR_T
#include <malloc.h>
#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>

#undef c_class
#define c_class Vector(VECTOR_T)

extern inline void Template(_Destroy)(VECTOR_T* _First, VECTOR_T* _Last);
extern inline void Template(_Destroy_single)(VECTOR_T* element);
extern inline void __rcat2(c_class, _move_ctor)(selfptr, c_class* other);
extern inline void Template(pop_front)(selfptr);

void Constructor(selfptr)
{
	self->capacity_ = 16;
	ALLOC_CHECK(self->data_ = (VECTOR_T*)malloc(16 * sizeof(VECTOR_T)));
	self->end_ = self->data_;
}
void Destructor(selfptr)
{
	if (!self->data_)return;
	Template(clear)(self);
	free(self->data_);
}

#ifndef __VERIFY__RANGE
#define __VERIFY__RANGE
#define _v_verify(...)

// Utility
size_t max_size()//shall be redone if Allocators
{
	return 32768;
}
size_t _Calculate_growth(size_t _Oldcapacity, size_t _Newsize) //MS STL Impl
{
	// given _Oldcapacity and _Newsize, calculate geometric growth
	const size_t _Max = max_size();

	if (_Oldcapacity > _Max - _Oldcapacity / 2)
	{
		return _Max; // geometric growth would overflow
	}

	const size_t _Geometric = _Oldcapacity + _Oldcapacity / 2;
	if (_Geometric < _Newsize)
	{
		return _Newsize; // geometric growth would be insufficient
	}

	return _Geometric; // geometric growth is sufficient
}
#endif


VECTOR_T* Template(_Emplace_reallocate)(selfptr)
{
	const size_t _Oldsize = (size_t)(self->end_ - self->data_);
	if (_Oldsize == max_size())
	{
		ALLOC_CHECK(false);
	}

	const size_t _Newsize = _Oldsize + 1;
	const size_t _Newcapacity = _Calculate_growth(_Oldsize, _Newsize);

	ALLOC_CHECK(self->data_ = (VECTOR_T*)realloc(self->data_, _Newcapacity * sizeof(VECTOR_T)));
	self->end_ = self->data_ + _Oldsize;
	self->capacity_ = _Newcapacity;
	return self->end_++;
}


// API Calls
VECTOR_T* Template(at)(const selfptr, size_t n)
{
	if ((size_t)(self->end_ - self->data_) <= n)
	{
		ALLOC_CHECK(false);
	}
	return self->data_ + n;
}
size_t Template(size)(const selfptr)
{
	return (size_t)(self->end_ - self->data_);
}
bool Template(empty)(const selfptr)
{
	return self->end_ == self->data_;
}
VECTOR_T* Template(front)(const selfptr)
{
	_v_verify(self->data_ != self->end_, "front() called on empty vector");
	return self->data_;
}
VECTOR_T* Template(back)(const selfptr)
{
	_v_verify(self->data_ != self->end_, "back() called on empty vector");
	return self->end_ - 1;
}

void Template(reserve)(selfptr, size_t new_cap)
{
	if (new_cap > self->capacity_)
	{
		size_t _OldSize = (size_t)(self->end_ - self->data_);
		self->capacity_ = new_cap;
		ALLOC_CHECK(self->data_ = (VECTOR_T*)realloc(self->data_,
			new_cap * sizeof(VECTOR_T)));
		self->end_ = self->data_ + _OldSize;
	}
}
void Template(shrink_to_fit)(selfptr)
{
	self->capacity_ = Template(size)(self);
	self->data_ = (VECTOR_T*)realloc(self->data_,
		self->capacity_ * sizeof(VECTOR_T));
	self->end_ = self->data_ + self->capacity_;
}
void Template(clear)(selfptr)
{
	Template(_Destroy)(self->data_, self->end_);
	self->end_ = self->data_;
}
void Template(erase)(selfptr, VECTOR_T* iterator)
{
	_v_verify(iterator >= self->data_ && self->end_ > iterator, "vector erase iterator outside range");
	Template(_Destroy_single)(iterator);
	while (iterator != self->end_ - 1)
	{
		memmove(iterator, iterator + 1, sizeof(*iterator));
		++iterator;
	}
	self->end_ = iterator;
}
void Template(erase_range)(selfptr, VECTOR_T* _Start, VECTOR_T* _End)
{
	_v_verify(_Start >= self->data_ && _End >= _Start && self->end_ >= _End,
		"vector erase iterator outside range");
	Template(_Destroy)(_Start, _End);
	memcpy(_Start, _End, (size_t)((uint8_t*)self->end_ - (uint8_t*)_End));
	self->end_ -= _End - _Start;
}
VECTOR_T* Template(push_back)(selfptr)
{
	// insert by perfectly forwarding into element at end, provide strong guarantee
	if (self->end_ != self->data_ + self->capacity_)
	{
		return self->end_++;
	}

	return Template(_Emplace_reallocate)(self);
}
void Template(pop_back)(selfptr)
{
	Template(_Destroy_single)(--self->end_);
}
#undef VECTOR_T
#include "VectorT.c"
#endif