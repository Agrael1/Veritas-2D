#pragma once
#include "Test.h"
#include "IndexedTriangleList.h"
#include "EngineCommons.h"

#define c_class CubeTex

typedef struct
{
	SVMVECTOR pos;
	VMVECTOR n;
	SVMVECTOR tc;
}virtual(Vertex);

class
{
	inherits(TestObject);
	struct GouraudVST* VS;
	struct GouraudPST* PS;
	IndexedTriangleList model;
};