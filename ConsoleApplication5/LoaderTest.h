#pragma once
#include "IndexedTriangleList.h"
#include "Test.h"
#include "EngineCommons.h"

#define c_class LoaderTest

typedef struct
{
	SVMVECTOR pos;
	VMVECTOR n;
}virtual(Vertex);

class
{
	inherits(TestObject);
	struct DefaultVS* VS;
	struct FlatLightGS* GS;
	struct ExperimentalPS* PS;
	struct IndexedTriangleList model;
};