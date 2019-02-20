#pragma once
#include "olcGameEngine.h"

struct GameDemo
{
	const void* class;
	GameEngine* GE;



};

void* GameDemo_ctor(void* self, va_list *ap)
{
	struct GameDemo *this = self;
	
}