#pragma once
#include "IndexedTriangleList.h"
#include "VeritasMath.h"
#include "Test.h"
#include "EngineCommons.h"

#define c_class Triangle

typedef struct
{
	SVMVECTOR pos;
	VMVECTOR n;
}virtual(Vertex);

class
{
	inherits(TestObject);
	struct GouraudVS* VS;
	struct GouraudPS* PS;
	IndexedTriangleList model;
};