#pragma once
#include <string.h>
#include "EngineCommons.h"

#define c_class String

typedef struct String
{
	privatev(
		size_t length;
		char* data;
	);
}String;

//API
String make_string(char* data);
String make_string_l(char* data, size_t length);
void string_remove(String* self);
void string_move(String* to, String* from);
void string_copy(String* self, String* from);
size_t string_length(String* self);
const char* c_str(String* self);

String string_fmt(const char* format, ...);
