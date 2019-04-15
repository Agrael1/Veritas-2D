#pragma once
#include "EngineCommons.h"

#define c_class StringStream

class
{
	const void* _class;

	methods(
		struct c_class* (*Append)(void* self, const char* str);
		struct c_class* (*AppendI)(void* self, const int num);
		char* (*str)(const void* self);
		char* (*EndStr)(void* self);
		const char* (*GetType)();
	);

	privatev(
		char* inStr;
	    DWord len;
	);
};