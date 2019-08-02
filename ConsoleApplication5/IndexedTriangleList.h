#pragma once
#include "VeritasMath.h"
#include <memory.h>
#include <assert.h>

typedef struct IndexedTriangleList
{
	size_t numVerts;
	size_t VSize;
	void* vertices;
	
	size_t numInds;
	size_t* indices;
}IndexedTriangleList;

static void CalcNormalsIndependentFlat(IndexedTriangleList* in, size_t normOffs)
{
	if (in->VSize < 2 * sizeof(VMVECTOR) || normOffs < sizeof(VMVECTOR))
		return;

	assert(in->numInds % 3 == 0 && in->numInds > 0);
	for (size_t i = 0; i < in->numInds; i += 3)
	{
		VMVECTOR* v0p = (VMVECTOR*)((Byte*)in->vertices + (i + 0) * in->VSize);
		VMVECTOR* v1p = (VMVECTOR*)((Byte*)in->vertices + (i + 1) * in->VSize);
		VMVECTOR* v2p = (VMVECTOR*)((Byte*)in->vertices + (i + 2) * in->VSize);

		VMVECTOR normal = VMVector3Normalize(VMVector3Cross(VMVectorSubtract(*v1p, *v0p), VMVectorSubtract(*v2p, *v0p)));

		memcpy_s((Byte*)v0p + normOffs, in->VSize-normOffs, &normal, sizeof(VMVECTOR));
		memcpy_s((Byte*)v1p + normOffs, in->VSize-normOffs, &normal, sizeof(VMVECTOR));
		memcpy_s((Byte*)v2p + normOffs, in->VSize-normOffs, &normal, sizeof(VMVECTOR));
	}
}