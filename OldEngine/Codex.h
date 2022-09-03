#pragma once
#include <HashMap.h>
#include "BindableBase.h"
#include "Templates.h"

typedef struct
{
	struct HashMap binds;
}Codex;

void DestroyCodex();
Codex* GetCodex();

// Automatically templated on c_class
#define  __Resolve(...)\
{\
	String key = virtual(GenerateUID)(__VA_ARGS__);\
	Codex* codex = GetCodex();\
	Node_HashMap* result = codex->binds.method->find(&codex->binds, key);\
	if (!result)\
	{\
		string_remove(&key);\
		return codex->binds.method->try_emplace(&codex->binds, key, c_class, __VA_ARGS__);\
	}\
	else\
	{\
		string_remove(&key);\
		return (shared_ptr(Bindable)*)(result->element); \
	} \
}	
