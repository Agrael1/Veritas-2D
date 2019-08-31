#pragma once
#include "IndexedTriangleList.h"
#include <memory.h>
#include <math.h>

static IndexedTriangleList Tri_Make(size_t VSize)
{
	void* vertices = malloc(3 * VSize);
	memcpy_s((char*)vertices + 0 * VSize, VSize, (float[]) { 0.0f, 0.8f, 0.0f, 1.0f }, 4 * sizeof(float)); // 0
	memcpy_s((char*)vertices + 1 * VSize, VSize, (float[]) { 0.8f, -0.8f, 0.0f, 1.0f }, 4 * sizeof(float)); // 1
	memcpy_s((char*)vertices + 2 * VSize, VSize, (float[]) { -0.8f, -0.8f, 0.0f, 1.0f }, 4 * sizeof(float)); // 2
	

	size_t* indices = malloc(3 * sizeof(size_t));
	memcpy(indices, (size_t[]) {0,1,2}, 3 * sizeof(size_t));

	IndexedTriangleList _ret = { 0 };
	_ret.vertices = vertices;
	_ret.VSize = VSize;
	_ret.indices = indices;
	_ret.numVerts = 3;
	_ret.numInds = 3;

	return _ret;
}