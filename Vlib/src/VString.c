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


static inline bool is_short(const String* self)
{
	return !self->flag;
}
static inline size_t max_size()
{
	return 1ull << ((sizeof(size_t) * 8) - 2);
}
static inline size_t _Calculate_growth(size_t _Oldcapacity, size_t _Newsize) //MS STL Impl
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
	if (is_short(self))
	{
		if (self->altrepr[sizeof(String) - 1])
		{
			self->altrepr[sizeof(String) - 1 - self->altrepr[sizeof(String) - 1]--] = c;
			self->altrepr[sizeof(String) - 1 - self->altrepr[sizeof(String) - 1]] = '\0';
		}
		else
		{
			size_t _NewSize = _Calculate_growth(sizeof(String), sizeof(String) + 1);
			char* _Replacement = malloc(_NewSize);
			memcpy(_Replacement, self, sizeof(String) - 1);

			_Replacement[sizeof(String) - 1] = c;
			_Replacement[sizeof(String)] = '\0';
			self->data = _Replacement;
			self->size = sizeof(String);
			self->capacity = _NewSize - 1;
			self->flag = 1;				//reasoning, 2^64 is not a good string to begin with
		}
		return;
	}
	if (self->size < self->capacity)
	{
		self->data[self->size++] = c;
		self->data[self->size] = '\0';
	}
	else
	{
		const size_t _Oldsize = self->size;
		if (_Oldsize == max_size() - 1)
		{
			ALLOC_CHECK(false);
		}

		const size_t _Newsize = _Oldsize + 1;
		const size_t _Newcapacity = _Calculate_growth(_Oldsize, _Newsize);

		ALLOC_CHECK(self->data = realloc(self->data, _Newcapacity));
		self->data[self->size++] = c;
		self->data[self->size] = '\0';
		self->capacity = _Newcapacity - 1;
	}
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

void string_insert_range(String* self, size_t start, const String* from, size_t offset_begin)
{
	size_t Mylength = string_length(self);
	size_t FromLength = string_length(from);
	if (offset_begin > FromLength) return;

	char* raw_inserter = (char*)c_str(self) + start;
	char* end_inserter = Mylength - start + raw_inserter;
	const char* from_offset = c_str(from) + offset_begin;

	while (raw_inserter != end_inserter && *from_offset)
	{
		*raw_inserter++ = *from_offset++;
	}
	while (*from_offset)
		string_push_back(self, *from_offset++);
}
void string_cat(String* self, const String* from)
{
	size_t start = string_length(self);
	string_insert_range(self, start, from, 0);
}
