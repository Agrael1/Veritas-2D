#pragma once
#include "Frame.h"
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
		void(*Draw)(selfptr, struct IndexedTriangleList* trilist);
	);

	struct Frame* gfx;
	struct VSBase* VS;
	struct GSBase* GS;
	struct PSBase* PS;

	__declspec(align(16))struct
	{
		VMVECTOR Offset;
		VMVECTOR Scale;
	}Global;
};