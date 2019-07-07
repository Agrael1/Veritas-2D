#pragma once
#include "IndexedTriangleList.h"
#include <memory.h>

static struct IndexedTriangleList Box_Make()
{
	const float side = 1.0f / 2.0f;

	Vertex* vertices = malloc(8*sizeof(Vertex));
	vertices[0].pos = (VMFLOAT3) { -side, -side, -side }; // 0
	vertices[1].pos = (VMFLOAT3) { side, -side, -side }; // 1
	vertices[2].pos = (VMFLOAT3) { -side, side, -side }; // 2
	vertices[3].pos = (VMFLOAT3) { side, side, -side }; // 3
	vertices[4].pos = (VMFLOAT3) { -side, -side, side }; // 4
	vertices[5].pos = (VMFLOAT3) { side, -side, side }; // 5
	vertices[6].pos = (VMFLOAT3) { -side, side, side }; // 6
	vertices[7].pos = (VMFLOAT3) { side, side, side }; // 7

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
	_ret.numVerts = 8;
	_ret.numInds = 36;

	return _ret;
}