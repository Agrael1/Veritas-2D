#pragma once
#include "Icosahedron.h"
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
	struct IndexedTriangleList model;
};