#pragma once
#include "IndexedTriangleList.h"
#include <memory.h>
#include <math.h>

static struct IndexedTriangleList Icosahedron_Make(size_t VSize)
{
	const float t = (1.0f + sqrtf(5.0f)) / 2.0f;

	void* vertices = malloc(12 * VSize);
	memcpy_s((char*)vertices + 0 * VSize, VSize, (float[]) { -1.0f, t, 0.0f }, 3 * sizeof(float)); // 0
	memcpy_s((char*)vertices + 1 * VSize, VSize, (float[]) { 1.0f, t, 0.0f }, 3 * sizeof(float)); // 1
	memcpy_s((char*)vertices + 2 * VSize, VSize, (float[]) { -1.0f, -t, 0.0f }, 3 * sizeof(float)); // 2
	memcpy_s((char*)vertices + 3 * VSize, VSize, (float[]) { 1.0f, -t, -0.0f }, 3 * sizeof(float)); // 3
	
	memcpy_s((char*)vertices + 4 * VSize, VSize, (float[]) { 0.0f, -1.0f, t }, 3 * sizeof(float)); // 4
	memcpy_s((char*)vertices + 5 * VSize, VSize, (float[]) { 0.0f, 1.0f, t }, 3 * sizeof(float)); // 5
	memcpy_s((char*)vertices + 6 * VSize, VSize, (float[]) { 0.0f, -1.0f, -t }, 3 * sizeof(float)); // 6
	memcpy_s((char*)vertices + 7 * VSize, VSize, (float[]) { 0.0f, 1.0f, -t }, 3 * sizeof(float)); // 7
	
	memcpy_s((char*)vertices + 8 * VSize, VSize, (float[]) { t, 0.0f, -1.0f }, 3 * sizeof(float)); // 8
	memcpy_s((char*)vertices + 9 * VSize, VSize, (float[]) { t, 0.0f, 1.0f }, 3 * sizeof(float)); // 9
	memcpy_s((char*)vertices + 10 * VSize, VSize, (float[]) { -t, 0.0f, -1.0f }, 3 * sizeof(float)); // 10
	memcpy_s((char*)vertices + 11 * VSize, VSize, (float[]) { -t, 0.0f, 1.0f }, 3 * sizeof(float)); // 11

	size_t* indices = malloc(60 * sizeof(size_t));
	memcpy(indices, (size_t[]) {
			0, 11, 5,	0, 5, 1,	0, 1, 7,	0, 7, 10,	0, 10, 11,
			1, 5, 9,	5, 11, 4,	11, 10, 2,	10, 7, 6,	7, 1, 8,
			3, 9, 4,	3, 4, 2,	3, 2, 6,	3, 6, 8,	3, 8, 9,
			4, 9, 5,	2, 4, 11,	6, 2, 10,	8, 6, 7,	9, 8, 1
	}, 60 * sizeof(size_t));

	struct IndexedTriangleList _ret = { 0 };
	_ret.vertices = vertices;
	_ret.VSize = VSize;
	_ret.indices = indices;
	_ret.numVerts = 12;
	_ret.numInds = 60;

	return _ret;
}
static struct IndexedTriangleList Icosahedron_MakeIndependent()
{
	//struct IndexedTriangleList temp = Icosahedron_Make(4);

	//Vertex* ReVertices = malloc(temp.numInds * sizeof(Vertex));
	//unsigned j = 0;
	//for (auto i = 0; i<temp.numInds;i++)
	//{
	//	ReVertices[j++] = temp.vertices[temp.indices[i]];
	//	temp.indices[i] = i;
	//}
	//free(temp.vertices);
	//temp.vertices = ReVertices;

	//return temp;
};