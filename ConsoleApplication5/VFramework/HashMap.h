#pragma once
#include "VString.h"
#include <stdint.h>

#define c_class HashMap

typedef struct virtual(Node)
{ 
	struct virtual(Node)* next;
	String key;
	uint8_t element[];
}virtual(Node);

class
{
	struct HashMap_base_* pimpl;
	size_t elements;
};
interface
{
	virtual(Node)* (*find)(selfptr, String key);
	virtual(Node)* (*find_c)(selfptr, const char *key);

	void* (*try_emplace)(selfptr, String key, ...);
	void (*insert)(selfptr, String key, void** value);

	void (*remove)(selfptr, String key);
};
ComposeType;

#ifndef HASH_MAP_IMPL
#undef c_class
#endif