#pragma once
#include "Frame.h"
#include "IndexedTriangleList.h"
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class Pipeline

class
{
	const void* _class;
	methods(
		void(*Draw)(selfptr, struct IndexedTriangleList* trilist);
	);

	struct ColorIndexPS* PS;
	struct Frame* gfx;

	VMMATRIX Transformation;
};