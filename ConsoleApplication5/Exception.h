#pragma once
#include <setjmp.h>
#include <memory.h>
#include "VString.h"
#include "EngineCommons.h"

#define c_class Exception

class
{
	GENERATED_DESC
	const char* file;
	
	methods(
		const char* (*GetType)(void);
		const char* (*what)(void* self);
		String (*GetOriginString)(const void* self);
	);

	String whatBuffer;
	unsigned int line;
};


typedef struct jmp_cache
{
	jmp_buf env;
	void* __e;
	bool validator;
}jmp_cache;

extern jmp_cache __cache__ex;


inline __declspec(noreturn) void throw(void* exception)
{
	__cache__ex.__e = exception;
	longjmp(__cache__ex.env, 1);
}

#define try jmp_buf __proxy_buf = {0}; memcpy(__proxy_buf, __cache__ex.env, sizeof(jmp_buf)); __cache__ex.validator = false;  if (!setjmp(__cache__ex.env)){

// unchecked catch, force convertion, unsafe
#define catch(type, name) } else {  \
	if(!__cache__ex.validator) \
		{memcpy( __cache__ex.env, __proxy_buf, sizeof(jmp_buf)); __cache__ex.validator = true;}\
	struct type* name = __cache__ex.__e;\
	if(name) 

// checked catch, conditional convertion, safe
#define catchc(type, name) } else if(typeid(__cache__ex.__e) == (type)){  \
	if(!__cache__ex.validator) \
		{memcpy( __cache__ex.env, __proxy_buf, sizeof(jmp_buf)); __cache__ex.validator = true;}\
	struct type* name = __cache__ex.__e;\
	if(name)

// 0 cares of type, just give me the block
#define finally } else {	if(!__cache__ex.validator) \
		{memcpy( __cache__ex.env, __proxy_buf, sizeof(jmp_buf)); __cache__ex.validator = true;}

#define endtry } if(__cache__ex.__e){delete(__cache__ex.__e); __cache__ex.__e = NULL;}

