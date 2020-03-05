#pragma once
#include "EngineCommons.h"

#define c_class BitSetBase

struct __BitSetBase
{
	GENERATED_DESC
	methods(
		bool(*Any)(const void* self);
		bool(*IsSet)(const void* self, size_t pos);
		void(*Set)(void* self, size_t pos);
		void(*Reset)(void* self, size_t pos);
		void(*FullReset)(void* self);
	);
	
	size_t Positions;
};

class 
{
	inherits(__BitSetBase);
	size_t BitArray[0];
};