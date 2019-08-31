#pragma once
#include "EngineCommons.h"

#define c_class HashMap

typedef struct _Node { 
	struct _Node *next;
	char *key;
	void* info;
}Node;

class
{
	GENERATED_DESC
	methods(
		Node*(*find)(selfptr, char *key);
		void*(*insert)(selfptr, char* key);
	);
	privatev(
		Node** list;
	);
	unsigned size;
	unsigned elsize;
};
