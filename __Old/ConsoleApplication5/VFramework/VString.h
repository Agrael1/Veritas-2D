#pragma once
#include <string.h>
#include "VDefs.h"

#define c_class String

typedef struct String
{
	privatev(
		size_t length;
		char* data;
	);
}String;

//API
String make_string(const char* data);
String make_string_l(const char* data, size_t length);
void string_remove(String* self);
String string_detatch(String* self);
void string_move(String* to, String* from);
void string_copy(String* self, const String* from);
bool string_cmp(const String* left, const String* right);
size_t string_length(const String* self);
const char* c_str(const String* self);

String string_fmt(const char* format, ...);

#ifndef VSTRING_IMPL
#undef c_class
#endif