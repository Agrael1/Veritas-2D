#pragma once
#include "Icosahedron.h"
#include "DefaultVS.h"
#include "Test.h"
#include "EngineCommons.h"

#define c_class Icosphere

typedef struct
{
	CVMVECTOR pos;
}virtual(Vertex);

class
{
	inherits(TestObject);
	struct DefaultVS* VS;
	struct IndexedTriangleList model;
};