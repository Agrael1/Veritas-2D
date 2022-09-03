#pragma once
#include "EngineCommons.h"

#define c_class Queue

class
{
	GENERATED_DESC
	void* insides;

	methods(
		void(*push)(selfptr, const void* smth);
		void(*pop)(selfptr, void* to);
		bool(*empty)(selfptr);
		void(*clear)(selfptr);
	);

	DWord Size;
	DWord Contains;
	DWord Capacity;
	DWord cur;
};