#pragma once
#include "SwapChain.h"
#include "VSBase.h"
#include "PSBase.h"
#include "EngineCommons.h"

#define c_class VLine

typedef struct
{
	size_t SV_PrimID;
}IAOut;

class
{
	struct SwapChain* gfx;

	struct VSBase VS;
	void (*VSApply)(void* self, void** _outVertex, void** _inVertex, void* buffers[2]);
	struct PSBase PS;
	CHAR_INFO(*PSApply)(void* self, void** _PixelIn);

	struct GSBase* GS;
	
	struct ConsoleWindow* Debug;

	__declspec(align(16))struct
	{
		VMVECTOR Offset;
		VMVECTOR Scale;
	};
	
	VMMATRIX projection;
	VMMATRIX camera;
};
interface
{
	void(*Draw)(selfptr, IndexedTriangleList* trilist);
};
ComposeType;

#ifndef VLINE_IMPL
#undef c_class
#endif // !VLINE_IMPL
