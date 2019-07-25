#pragma once
#include "Icosphere.h"
#include "VeritasMath.h"
#include "EngineCommons.h"



#define c_class DefaultVS

class
{
	const void* _class;

	methods(
		void(*Apply)(selfptr, VMVECTOR* _out, struct IndexedTriangleList* _in);
	);

	VMMATRIX Transformation;
};