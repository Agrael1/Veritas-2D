#pragma once
#include "EngineCommons.h"

#define c_class Queue

class
{
	const void* _class;
	void* insides;
	DWord Size;
	DWord Contains;
	DWord Capacity;
	DWord cur;

	methods(
		void(*push)(self, smth);
		void(*pop)(struct c_class* self, void* to);
		bool(*empty)(struct c_class* self);
		void(*clear)(struct c_class* self);
	);
};