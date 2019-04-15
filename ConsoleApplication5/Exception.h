#pragma once
#include "EngineCommons.h"

#define c_class Exception

class
{
	const void* _class;
	int line;
	const char* file;
	
	methods(
		char* (*GetType)();
		char* (*what)(void* self);
	);

	char* whatBuffer;
};