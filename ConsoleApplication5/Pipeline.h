#pragma once
#include "Frame.h"
#include "IndexedTriangleList.h"
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class Pipeline

class
{
	GENERATED_DESC
	methods(
		void(*Draw)(selfptr, struct IndexedTriangleList* trilist);
	);

	struct ColorIndexPS* PS;
	struct Frame* gfx;
	struct VSBase* VS;
	struct FlatLightGS* GS;
};