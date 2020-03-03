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


extern jmp_buf env;
extern struct Exception *__e;


inline void throw(void* exception)
{
	__e = exception;
	longjmp(env,1);
}

#define try jmp_buf __proxy_buf; memcpy(__proxy_buf, env, sizeof(jmp_buf)); if (!setjmp(env))  
#define catch(x,y) else {  \
	memcpy( env, __proxy_buf, sizeof(jmp_buf)); \
	struct x* y = __e;\
	if(typeOf(y) == #x) 
#define catchn(x,y)\
	struct x* y = __e;\
	if(typeOf(y) == #x)
#define endtry } if(__e){delete(__e); __e=NULL;}

