#pragma once
#include "Vector.h"
#include "SharedPtr.h"
#include "VeritasMath.h"
#include "EngineCommons.h"
#include "IndexedTriangleList.h"

#define c_class Drawable

class
{
	GENERATED_DESC
	methods(
		VMMATRIX(*GetTransformXM)(void* self);
		void(*AddBind)(selfptr, shared_ptr bindable);
		void(*Draw)(selfptr, struct VLine* gfx);
	);
	struct Vector Binds;
	IndexedTriangleList trilist;
};