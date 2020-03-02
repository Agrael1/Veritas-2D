#pragma once
#include "HashMap.h"

typedef struct
{
	struct HashMap binds;
}Codex;

void InitializeCodex();
void DestroyCodex();

// Automatically templated on c_class
#define __Resolve(infoset,...)													\
const char* key = virtual(GenerateUID)(infoset);								\
Node* result = codex.binds.method->find(&codex.binds, key);						\
shared_ptr* insert;																\
if (!result)																	\
{																				\
	insert = codex.binds.method->insert(&codex.binds, key);						\
	*insert = make_shared(c_class, __VA_ARGS__);								\
	return *insert;																\
}																				\
else																			\
{																				\
	insert = (shared_ptr*)(&result->info);										\
	insert->control->counter++;													\
	return *insert;																\
}																				
	
	
	
	
	
	
	
	
	
	
	
	
