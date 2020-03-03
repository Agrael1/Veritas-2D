#include "Class.h"
#include "Exception.h"

jmp_buf env = { 0 }; 
struct Exception* __e = NULL;


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
	String origin = _GetOriginString(self);
	String out = string_fmt("%s\n%s", virtual(GetType), c_str(&origin));
	string_move(&self->whatBuffer, &out);

	string_remove(&origin);
	return c_str(&self->whatBuffer);
}

VirtualTable{
	.GetType = virtual(GetType),
	.what = virtual(what),
	.GetOriginString = _GetOriginString
};

Constructor(void* self, va_list* ap)
{
	account(self);
	assignMethodTable(this);
	this->line = va_arg(*ap, unsigned);
	this->file = va_arg(*ap, const char*);
	this->whatBuffer = (String){ 0 };
	return this;
}
Destructor(void* self)
{
	account(self);
	string_remove(&this->whatBuffer);
	return this;
}
ENDCLASSDESC