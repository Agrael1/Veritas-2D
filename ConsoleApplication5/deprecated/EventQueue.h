#pragma once
#include "EngineCommons.h"

#define c_class EventQueue

class
{
	GENERATED_DESC
	void* Container;
	methods(
		void* (*Read)(selfptr);
		void* (*GetNext)(selfptr);
		void (*Wipe)(selfptr);
	);

	unsigned ElementSize;
	unsigned Contains;
	unsigned Capacity;
	unsigned cur;
};