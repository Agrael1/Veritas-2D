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

	DWord ElementSize;
	DWord Contains;
	DWord Capacity;
	DWord cur;
};