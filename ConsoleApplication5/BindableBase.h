#pragma once
#include "Codex.h"
#include "VLine.h"
#include "SharedPtr.h"
#include "EngineCommons.h"

#define c_class Bindable

extern Codex codex;

class
{
	GENERATED_DESC
	void (*Bind)(void* self, struct VLine* gfx);
};