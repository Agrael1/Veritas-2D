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


inline void throw(void* exception)
{
	__cache__ex.__e = exception;
	longjmp(__cache__ex.env, 1);
}

#define try jmp_buf __proxy_buf = {0}; memcpy(__proxy_buf, __cache__ex.env, sizeof(jmp_buf)); __cache__ex.validator = false; if (!setjmp(__cache__ex.env))

#define catch(type, name) else {  \
	if(!__cache__ex.validator) \
		{memcpy( __cache__ex.env, __proxy_buf, sizeof(jmp_buf)); __cache__ex.validator = true;}\
	struct type* name = __cache__ex.__e;\
	if(name) 

//checked catch
#define catchc(type, name)\
	if(!__cache__ex.validator) \
		{memcpy( __cache__ex.env, __proxy_buf, sizeof(jmp_buf)); __cache__ex.validator = true;}\
	struct type* name = __cache__ex.__e;\
	if(typeid(name) == type)

#define endtry } if(__cache__ex.__e){delete(__cache__ex.__e); __cache__ex.__e = NULL;}

