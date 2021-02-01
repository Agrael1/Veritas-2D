/**
 * @file vstring.c
 * @author Ilya Doroshenko
 * @brief String API implementation
 */
#define VSTRING_IMPL
#include "VString.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>


static inline bool is_short(const String* self)
{
	return !self->flag;
}
static inline size_t max_size()
{
	return 1ull << ((sizeof(size_t) * 8) - 2);
}
static inline size_t Calculate_growth(size_t Oldcapacity, size_t Newsize) //MS STL Impl
{
	// given _Oldcapacity and _Newsize, calculate geometric growth
	const size_t Max = max_size();

	if (Oldcapacity > Max - Oldcapacity / 2)
	{
		return Max; // geometric growth would overflow
	}

	const size_t Geometric = Oldcapacity + Oldcapacity / 2;
	if (Geometric < Newsize)
	{
		return Newsize; // geometric growth would be insufficient
	}

	return Geometric; // geometric growth is sufficient
}
static void Reallocate_for(String* self, size_t InputSize)
{
	const size_t Oldsize = string_length(self);
	const size_t Newsize = Oldsize + InputSize;

	if (Newsize <= string_capacity(self))return;
	const size_t Newcapacity = Calculate_growth(Oldsize, Newsize);

	if (!is_short(self))
	{
		if (Oldsize == max_size() - 1)
		{
			assert(false);
		}

		ALLOC_CHECK(self->data = realloc(self->data, Newcapacity));
		self->capacity = Newcapacity - 1;
		return;
	}

	char* Replacement = malloc(Newcapacity);
	ALLOC_CHECK(Replacement);
	memcpy(Replacement, self, Oldsize - 1);

	self->data = Replacement;
	self->size = Oldsize;
	self->capacity = Newcapacity - 1;
	self->flag = 1;				//reasoning, 2^64 is not a good string to begin with
}

void Constructor(String* self, const char* data, size_t size)
{
	if (!data)
	{
		self->altrepr[0] = '\0';
		self->altrepr[sizeof(String) - 1] = sizeof(String) - 1;
		return;
	}
	size_t length = size;
	if (!length) length = strlen(data);

	if (length < sizeof(String))
	{
		memcpy(self, data, length);
		self->altrepr[length] = 0;
		self->altrepr[sizeof(String) - 1] = (char)(sizeof(String) - length - 1);
		return;
	}
	self->capacity = length + 1;
	self->flag = 1;				//reasoning, 2^64 is not a good string to begin with
	ALLOC_CHECK(self->data = malloc(length + 1));						//for now fixed size
	if (!self->data) return;

	strcpy(self->data, data);
	self->size = length;
}
void Destructor(String* self)
{
	if (!is_short(self) && self->data)
		free(self->data);
}

extern inline String string_move(String* self);
extern inline void string_assign(String* self, String from);
extern inline void string_move_assign(String* self, String* from);
extern inline void string_copy_assign(String* to, String* from);
extern inline const char* c_str(const String* self);
extern inline char string_at(const String* self, size_t n);

extern inline size_t string_length(const String* self);
extern inline bool string_empty(const String* self);
extern inline size_t string_capacity(const String* self);
extern inline bool string_cmp(const String* left, const String* right);

extern inline StringView string_view(const String* self);
extern inline StringView string_view_c(const char* str);
extern inline StringView substr(StringView str, size_t start, size_t count);

String string_copy(const String* from)
{
	if (!from->flag)
		return *from;
	char* dupstr = malloc(from->size + 1);
	ALLOC_CHECK(dupstr);
	return CLTYPE(String) { { strcpy(dupstr, from->data), from->size, from->size, from->flag} };
}

void string_push_back(String* self, char c)
{
	if (is_short(self) && self->altrepr[sizeof(String) - 1])
	{
		self->altrepr[sizeof(String) - 1 - self->altrepr[sizeof(String) - 1]--] = c;
		self->altrepr[sizeof(String) - 1 - self->altrepr[sizeof(String) - 1]] = '\0';
		return;
	}

	Reallocate_for(self, 1);
	self->data[self->size++] = c;
	self->data[self->size] = '\0';
}
void string_clear(String* self)
{
	if (is_short(self))
	{
		self->data = NULL;
		self->altrepr[sizeof(String) - 1] = sizeof(String) - 1;
	}
	else
	{
		self->data[0] = 0;
		self->size = 0;
	}
}

void string_insert_range(String* self, size_t start, StringView from)
{
	size_t Mylength = string_length(self);

	char* raw_inserter = (char*)c_str(self) + start;
	char* end_inserter = Mylength - start + raw_inserter;

	while (raw_inserter != end_inserter && *from.data)
	{
		*raw_inserter++ = *from.data++;
	}
	while (*from.data)
		string_push_back(self, *from.data++);
}
void string_append(String* self, const char* input)
{
	StringView x = string_view_c(input);
	Reallocate_for(self, x.size);

	if (is_short(self))
	{
		while(*input)
			self->altrepr[sizeof(String) - 1 - self->altrepr[sizeof(String) - 1]--] = *input++;
		self->altrepr[sizeof(String) - 1 - self->altrepr[sizeof(String) - 1]] = '\0';
		return;
	}
	memcpy(self->data, x.data, x.size);
	self->size += x.size;
	self->data[self->size] = '\0';
}
void string_cat(String* self, const String* from)
{
	size_t start = string_length(self);
	string_append(self, c_str(from));
}

