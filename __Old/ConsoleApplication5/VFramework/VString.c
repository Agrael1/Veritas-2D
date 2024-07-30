#define VSTRING_IMPL
#include "VString.h"
#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>


String make_string_l(const char* data, size_t length)
{
	String s;
	*(struct String_private*)(&s.__internal_prtb_String) = (struct String_private){
	.length = length,
	.data = _strdup(data)
	};
	return s;
}
String make_string(const char* data)
{
	return make_string_l(data, strlen(data));
}

void string_remove(String* self)
{
	private.length = 0;
	if (private.data) free(private.data);
}
String string_detatch(String* self)
{
	String _out = *self;
	private.data = NULL;
	return _out;
}
void string_move(String* self, String* from)
{
	string_remove(self);
	*self = *from;
	(*(struct __rconcat(c_class, _private)*)(from->virtual(__internal_prtb))).data = NULL;
}
void string_copy(String* self, const String* from)
{
	string_remove(self);
	*self = *from;
	private.data = _strdup(c_str(from));
}
bool string_cmp(const String* left, const String* right)
{
	if(string_length(left)!= string_length(right))
		return false;
	if (strcmp(c_str(left), c_str(right)) == 0)
		return true;
	return false;
}
size_t string_length(const String* self)
{
	return private.length;
}
const char* c_str(const String* self)
{
	return private.data;
}

String string_fmt(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	char inline_buffer[512];
	vsprintf_s(inline_buffer, 512, format, ap);
	va_end(ap);
	return make_string(inline_buffer);
}