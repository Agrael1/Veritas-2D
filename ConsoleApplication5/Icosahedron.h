#pragma once
#include "IndexedTriangleList.h"
#include <memory.h>
#include <math.h>

static struct IndexedTriangleList Icosahedron_Make()
{
	const float t = (1.0f + sqrtf(5.0f)) / 2.0f;

	Vertex* vertices = malloc(12*sizeof(Vertex));
	vertices[0].pos = (VMFLOAT3) { -1.0f, t, 0.0f }; // 0
	vertices[1].pos = (VMFLOAT3) { 1.0f, t, 0.0f }; // 1
	vertices[2].pos = (VMFLOAT3) { -1.0f, -t, 0.0f }; // 2
	vertices[3].pos = (VMFLOAT3) { 1.0f, -t, -0.0f }; // 3
			  
	vertices[4].pos = (VMFLOAT3) { 0.0f, -1.0f, t }; // 4
	vertices[5].pos = (VMFLOAT3) { 0.0f, 1.0f, t }; // 5
	vertices[6].pos = (VMFLOAT3) { 0.0f, -1.0f, -t }; // 6
	vertices[7].pos = (VMFLOAT3) { 0.0f, 1.0f, -t }; // 7
						  
	vertices[8].pos = (VMFLOAT3) { t, 0.0f, -1.0f }; // 8
	vertices[9].pos = (VMFLOAT3) { t, 0.0f, 1.0f }; // 9
	vertices[10].pos = (VMFLOAT3) { -t, 0.0f, -1.0f }; // 10
	vertices[11].pos = (VMFLOAT3) { -t, 0.0f, 1.0f }; // 11

	size_t* indices = malloc(60 * sizeof(size_t));
	memcpy(indices, (size_t[]) {
			0, 11, 5,	0, 5, 1,	0, 1, 7,	0, 7, 10,	0, 10, 11,
			1, 5, 9,	5, 11, 4,	11, 10, 2,	10, 7, 6,	7, 1, 8,
			3, 9, 4,	3, 4, 2,	3, 2, 6,	3, 6, 8,	3, 8, 9,
			4, 9, 5,	2, 4, 11,	6, 2, 10,	8, 6, 7,	9, 8, 1
	}, 60 * sizeof(size_t));

	struct IndexedTriangleList _ret = { 0 };
	_ret.vertices = vertices;
	_ret.indices = indices;
	_ret.numVerts = 12;
	_ret.numInds = 60;

	return _ret;
}