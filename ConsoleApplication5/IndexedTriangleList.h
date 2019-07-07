#pragma once
#include "VTypes.h"
#include "BitField.h"

typedef struct 
{
	VMFLOAT3 pos;
}Vertex;


struct IndexedTriangleList
{
	size_t numVerts;
	Vertex* vertices;

	size_t numInds;
	size_t* indices;
};