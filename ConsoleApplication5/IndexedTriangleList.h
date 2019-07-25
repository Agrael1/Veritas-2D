#pragma once
#include "VTypes.h"

struct IndexedTriangleList
{
	size_t numVerts;
	size_t VSize;
	void* vertices;
	
	size_t numInds;
	size_t* indices;
};