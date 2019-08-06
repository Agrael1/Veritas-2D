#pragma once
#include "IndexedTriangleList.h"
#include <memory.h>

static struct IndexedTriangleList Box_Make(size_t VSize)
{
	const float side = 1.0f / 2.0f;

	void* vertices = malloc(8 * sizeof(VSize));
	memcpy_s((char*)vertices + 0 * VSize, VSize, (float[]) { -side, -side, -side, 1.0f }, 4 * sizeof(float)); // 0
	memcpy_s((char*)vertices + 0 * VSize, VSize, (float[]) { side, -side, -side, 1.0f }, 4 * sizeof(float)); // 1
	memcpy_s((char*)vertices + 0 * VSize, VSize, (float[]) { -side, side, -side, 1.0f }, 4 * sizeof(float)); // 2
	memcpy_s((char*)vertices + 0 * VSize, VSize, (float[]) { side, side, -side, 1.0f }, 4 * sizeof(float)); // 3
	memcpy_s((char*)vertices + 0 * VSize, VSize, (float[]) { -side, -side, side, 1.0f }, 4 * sizeof(float)); // 4
	memcpy_s((char*)vertices + 0 * VSize, VSize, (float[]) { side, -side, side, 1.0f }, 4 * sizeof(float)); // 5
	memcpy_s((char*)vertices + 0 * VSize, VSize, (float[]) { -side, side, side, 1.0f }, 4 * sizeof(float)); // 6
	memcpy_s((char*)vertices + 0 * VSize, VSize, (float[]) { side, side, side, 1.0f }, 4 * sizeof(float)); // 7

	size_t* indices = malloc(36 * sizeof(size_t));
	memcpy(indices, (size_t[]) {
		0, 2, 1, 2, 3, 1,
			1, 3, 5, 3, 7, 5,
			2, 6, 3, 3, 6, 7,
			4, 5, 7, 4, 7, 6,
			0, 4, 2, 2, 4, 6,
			0, 1, 4, 1, 5, 4
	}, 36 * sizeof(size_t));

	struct IndexedTriangleList _ret = { 0 };
	_ret.vertices = vertices;
	_ret.indices = indices;
	_ret.VSize = VSize;
	_ret.numVerts = 8;
	_ret.numInds = 36;

	return _ret;
}