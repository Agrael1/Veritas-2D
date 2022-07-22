/**
 * @file vstring.c
 * @author Ilya Doroshenko
 * @brief String API implementation
 */
#define _CRT_SECURE_NO_WARNINGS
#define VSTRING_IMPL
#include "VString.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>


DefaultOperatorsI


static inline size_t max_size()
{
	return (size_t)((1ull << (sizeof(size_t) * CHAR_BIT - 1)) - 1);
}
static inline size_t calculate_growth(size_t Oldcapacity, size_t Newsize) //MS STL Impl
{
	// given _Oldcapacity and _Newsize, calculate geometric growth
	const size_t Max = max_size();
	const size_t Growth = Oldcapacity >> 1; // /2
	const size_t Geometric = Oldcapacity + Growth;

	if (Geometric > Max)
		return Max; // geometric growth would overflow

	if (Geometric < Newsize)
		return Newsize; // geometric growth would be insufficient

	return Geometric; // geometric growth is sufficient
}
static void reallocate_for(String* self, size_t InputSize)
{
	const size_t Oldsize = string_length(self) + 1; //term \0
	const size_t Newsize = Oldsize + InputSize;

	if (Newsize <= string_capacity(self))return;
	const size_t Newcapacity = calculate_growth(Oldsize, Newsize);

	if (self->large)
	{
		if (Oldsize == max_size()){
			assert(false);
			return;
		}

		ALLOC_CHECK(self->data = realloc(self->data, Newcapacity));
		self->capacity = Newcapacity;
		return;
	}

	char* Replacement = malloc(Newcapacity);
	ALLOC_CHECK(Replacement);
	memcpy(Replacement, self, Oldsize);

	self->data = Replacement;
	self->size = Oldsize - 1;
	self->capacity = Newcapacity;
	self->large = 1;				//reasoning, 2^63 is not a good string to begin with
}

void Constructor(String* self, const char* data, size_t length)
{
	if (!data)
	{
		self->shortstr[0] = 0;
		self->last = sizeof(String) - 1;
		return;
	}
	if (!length) length = strlen(data);

	if (length < sizeof(String))
	{
		memcpy(self, data, length);
		self->shortstr[length] = 0;
		self->last = (char)(sizeof(String) - length - 1);
		return;
	}
	self->capacity = length + 1; //term \0
	self->large = 1;
	ALLOC_CHECK(self->data = malloc(self->capacity));						//for now fixed size
	if (!self->data) return;

	strcpy(self->data, data);
	self->size = length;
}
void Destructor(String* self)
{
	if (self->large && self->data)
		free(self->data);
}
void Move(String* self, String* other)
{
	*self = *other;
	other->large = 0;
	Constructor(other, NULL, 0);
}
void Copy(String* self, const String* other)
{
	*self = *other;
	if (other->large)
	{
		ALLOC_CHECK(self->data = malloc(self->capacity));
		strcpy(self->data, other->data);
	}
}

extern inline const char* c_str(const String* self);
extern inline char string_at(const String* self, size_t n);

extern inline size_t string_length(const String* self);
extern inline bool string_empty(const String* self);
extern inline size_t string_capacity(const String* self);
extern inline bool string_cmp(const String* left, const String* right);

extern inline StringView string_view(const String* self);
extern inline StringView string_view_c(const char* str);
extern inline StringView substr(StringView str, size_t start, size_t count);

void string_push_back(String* self, char c)
{
	if (!self->large && self->last)
	{
		self->shortstr[sizeof(String) - 1 - self->last--] = c;
		self->shortstr[sizeof(String) - 1 - self->last] = '\0';
		return;
	}

	reallocate_for(self, 1);
	self->data[self->size++] = c;
	self->data[self->size] = '\0';
}
void string_clear(String* self)
{
	if (!self->large)
		self->last = sizeof(String) - 1;
	else
		self->size = 0;
}

void string_append(String* self, const char* input)
{
	StringView x = string_view_c(input);
	reallocate_for(self, x.size);

	if (!self->large)
	{
		while(*input)
			self->shortstr[sizeof(String) - 1 - self->last--] = *input++;
		self->shortstr[sizeof(String) - 1 - self->last] = '\0';
		return;
	}
	memcpy(self->data + self->size, x.data, x.size);
	self->size += x.size;
	self->data[self->size] = '\0';
}
void string_prepend(String* self, const char* input)
{
	StringView x = { input, strlen(input) };
	reallocate_for(self, x.size);

	if (!self->large)
	{
		memmove(self->shortstr + x.size, self->shortstr, sizeof(String) - self->last);
		memcpy(self->shortstr, x.data, x.size);
		if(self->last)self->last -= (char)x.size;
		return;
	}
	memmove(self->data + x.size, self->data, self->size + 1);
	memcpy(self->data, x.data, x.size);
	self->size += x.size;
}
void string_pop_front(String* self)
{
	if (string_empty(self))
	{
		assert(false && __FUNCTION__ "Operation performed on empty string");
		return;
	}
	if (!self->large)
	{
		memmove(self->shortstr, self->shortstr + 1, sizeof(String) - 1 - self->last - 1);
		self->shortstr[sizeof(String) - 1 - self->last - 1] = '\0';
		self->last++;
		return;
	}
	memmove(self->data, self->data + 1, self->size--);
}

void string_reserve(String* self, size_t count)
{
	reallocate_for(self, count);
}

void string_cat(String* self, const String* from)
{
	string_append(self, c_str(from));
}

