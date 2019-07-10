#include "Class.h"
#include "StringStream.h"
#include "Exception.h"
#include <malloc.h>

setupException

const char* virtual(GetType)() 
{
	return "Veritas Exception\n\r";
}
char* _GetOriginString(const selfptr)
{
	const account(self);
	struct StringStream *oss = new(StringStream);
	oss->method->Append(oss, "[File]: ")->method->Append(oss,this->file)
		->method->Append(oss,"\n\r[Line]: ")->method->AppendI(oss,this->line);
	return oss->method->EndStr(oss);
}
char* virtual(what) (selfptr)
{
	account(self);
	struct StringStream *oss = new(StringStream);
	char* _proxy = _GetOriginString(this);
	oss->method->Append(oss,this->method->GetType())->method->Append(oss, _proxy);

	this->whatBuffer = oss->method->EndStr(oss);

	free(_proxy);
	return this->whatBuffer;
}

constructMethodTable(
	.GetType = virtual(GetType),
	.what = virtual(what),
	.GetOriginString = _GetOriginString
);

Constructor(void* self, va_list* ap)
{
	account(self);
	assignMethodTable(this);
	this->line = va_arg(*ap, Word);
	this->file = va_arg(*ap, const char*);
	this->whatBuffer = NULL;
	return this;
}
Destructor(void* self)
{
	account(self);
	if (this->whatBuffer)
		free(this->whatBuffer);
	return this;
}
ENDCLASSDESC