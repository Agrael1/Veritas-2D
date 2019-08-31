#pragma once
#include "EngineCommons.h"

#define c_class Vector

class
{
	GENERATED_DESC

	methods(
		void*(*get_next)(selfptr);
		void*(*get)(selfptr, size_t index);
	);
	void* array;
	size_t elements;
	size_t size;
};