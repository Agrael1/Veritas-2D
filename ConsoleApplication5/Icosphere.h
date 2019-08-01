#pragma once
#include "Icosahedron.h"
#include "Test.h"
#include "EngineCommons.h"

#define c_class Icosphere

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