#include "VString.h"
#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>


String make_string_l(char* data, size_t length)
{
	String s;
	*(struct String_private*)(&s.__internal_prtb_String) = (struct String_private){
	.length = length,
	.data = _strdup(data)
	};
	return s;
}
String make_string(char* data)
{
	return make_string_l(data, strlen(data));
}

void string_remove(String* self)
{
	private.length = 0;
	if (private.data) free(private.data);
}
void string_move(String* self, String* from)
{
	string_remove(self);
	*self = *from;
	(*(struct _private*)(from->virtual(__internal_prtb))).data = NULL;
}
void string_copy(String* self, String* from)
{
	string_remove(self);
	*self = *from;
	private.data = _strdup((*(struct _private*)(from->virtual(__internal_prtb))).data);
}
size_t string_length(String* self)
{
	return private.length;
}
const char* c_str(String* self)
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