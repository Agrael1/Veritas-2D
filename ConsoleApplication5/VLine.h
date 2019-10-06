#pragma once
#include "Frame.h"
#include "VSBase.h"
#include "EngineCommons.h"

#define c_class VLine

typedef struct
{
	size_t SV_PrimID;
}IAOut;

class
{
	GENERATED_DESC

	methods(
		void(*Draw)(selfptr, IndexedTriangleList* trilist);
	);

	struct SwapChain* gfx;

	struct VSBase VS;
	struct GSBase* GS;
	struct PSBase* PS;
	struct ConsoleWindow* Debug;

	__declspec(align(16))struct
	{
		VMVECTOR Offset;
		VMVECTOR Scale;
	}Global;
	
	VMMATRIX projection;
	VMMATRIX camera;
};