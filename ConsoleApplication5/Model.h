#pragma once
#include "Vector.h"
#include "Drawable.h"
#include "EngineCommons.h"

#define c_class Mesh

class
{
	inherits(Drawable);
	void(*Draw)(selfptr, struct VLine* gfx, FXMMATRIX accumulatedTransform);
	VMFLOAT4X4A transform;
};

#undef c_class
#define c_class MNode

class
{
	GENERATED_DESC
	void(*Draw)(selfptr, struct VLine* gfx, FXMMATRIX accumulatedTransform);

	struct Mesh** meshPtrs;
	char* name;
	size_t nmeshes;
	int id;

	struct Vector childPtrs;
	VMFLOAT4X4A transform;
	VMFLOAT4X4A appliedTransform;
};

#undef c_class
#define c_class Model

class
{
	GENERATED_DESC
	methods(
		void(*Draw)(const selfptr, struct VLine* gfx);
	);
	struct MNode* pRoot;
	struct Mesh** meshPtrs;
	size_t nmeshes;
};