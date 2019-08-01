#pragma once
#include "Frame.h"
#include "IndexedTriangleList.h"
#include "VeritasMath.h"
#include "EngineCommons.h"

typedef struct
{
	size_t SV_PrimID;
}IAOut;

#define c_class Pipeline

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
};