#pragma once
#include "VDefs.h"

#define c_class __BitSetBase

struct __BitSetBase_base
{
	size_t Positions;
};
interface
{
	bool(*Any)(const void* self);
	bool(*IsSet)(const void* self, size_t pos);
	void(*Set)(void* self, size_t pos);
	void(*Reset)(void* self, size_t pos);
	void(*FullReset)(void* self);
};
ComposeType;

#undef c_class
#define c_class BitSetBase

class 
{
	inherit(__BitSetBase);
	size_t BitArray[0];
};
interface
{
	implements(__BitSetBase);
};
ComposeType;

