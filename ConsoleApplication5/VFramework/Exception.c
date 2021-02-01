#define EXCEPTION_IMPL
#include "Exception.h"
#include "Class.h"
#include <stdlib.h>

String _GetOriginString(const selfptr)
{
	return string_fmt("[File]: %s\n[Line]: %u", self->file, self->line);
}
const char* virtual(GetType)(void) 
{
	return "Veritas Exception\n\r";
}
const char* virtual(what)(selfptr)
{
	if (!c_str(&self->whatBuffer))
	{
		String origin = _GetOriginString(self);
		self->whatBuffer = string_fmt("%s\n%s", virtual(GetType)(), c_str(&origin));
		string_remove(&origin);
	}
	return c_str(&self->whatBuffer);
}

VirtualTable{
	.GetType = virtual(GetType),
	.what = virtual(what),
	.GetOriginString = _GetOriginString
};
Constructor(selfptr, va_list* ap)
{
	InitializeVtable(self);
	self->line = va_arg(*ap, unsigned);
	self->file = va_arg(*ap, const char*);
	self->whatBuffer = (String){ 0 };
}
Destructor(selfptr)
{
	string_remove(&self->whatBuffer);
}
ENDCLASSDESC

void _throw(void* exception)
{
	if (*__cache__ex.env == NULL) abort(); //call for std::terminate

	__cache__ex.__e = exception;
	__cache__ex.validator = false;
		
	__trim_except();
	longjmp(*__cache__ex.env, 1);
}
extern inline void _envbuf_xchg(jmp_buf* pOldbuf);