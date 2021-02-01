#pragma once
#include "VString.h"
#include "Unwinder.h"
#include "VDefs.h"

#define c_class Exception

class
{
	const char* file;
	String whatBuffer;
	unsigned int line;
};	
interface
{
	const char* (*GetType)(void);
	const char* (*what)(void* self);
	String	(*GetOriginString)(const void* self);
};
ComposeType;

void __declspec(noreturn) _throw(void* exception);
inline void _envbuf_xchg(jmp_buf* pOldbuf)
{
	if (!__cache__ex.validator) 
	{
		__cache__ex.env = pOldbuf;
		__cache__ex.validator = true;
	}
}

#define throw _throw

#define try \
{\
	pContext_entry __old_C = __cache__ex.current_context;\
	Context_entry __context__e = _Make_Context_Entry();\
	__cache__ex.current_context = &__context__e;\
	jmp_buf __proxy_buf = { 0 };\
	jmp_buf* _env_ = __cache__ex.env;\
	__cache__ex.env = &__proxy_buf;\
	__cache__ex.validator = false;\
	if (!setjmp(__proxy_buf)){

// checked catch, conditional convertion, safe
#define catchc(type, name)\
	}\
	else if (typeid(__cache__ex.__e) == (uintptr_t)(type))\
	{\
		_envbuf_xchg(_env_); __cache__ex.current_context = __old_C; \
		struct type* name = __cache__ex.__e; __cache__ex.__e = NULL;\
		if (name)

// unchecked catch, force convertion, unsafe
#define catch(type, name) } else {  \
	_envbuf_xchg(_env_); __cache__ex.current_context = __old_C;\
	struct type* name = __cache__ex.__e; __cache__ex.__e = NULL;\
	if(name) 

// 0 cares of type, just give me the block
#define catch_any } else {_envbuf_xchg(_env_); __cache__ex.current_context = __old_C;

#define endtry } if(__cache__ex.__e){delete(__cache__ex.__e); __cache__ex.__e = NULL;} _envbuf_xchg(_env_);__cache__ex.current_context = __old_C;} 

#ifndef EXCEPTION_IMPL
#undef c_class
#endif