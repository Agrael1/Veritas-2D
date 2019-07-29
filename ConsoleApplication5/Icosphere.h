#pragma once
#include "Icosahedron.h"
#include "DefaultVS.h"
#include "FlatLightGS.h"
#include "ExperimentalPS.h"
#include "Test.h"
#include "EngineCommons.h"

#define c_class Icosphere

typedef struct
{
	SVMVECTOR pos;
}virtual(Vertex);

class
{
	inherits(TestObject);
	struct DefaultVS* VS;
	struct FlatLightGS* GS;
	struct ExperimentalPS* PS;
	struct IndexedTriangleList model;
};