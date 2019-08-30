#pragma once
#define dIDESINGLE
#include "VeritasMath.h"
#include <ode/ode.h>
#include "EngineCommons.h"

#define c_class Physics

typedef struct 
{
	dBodyID Body;  // the dynamics body
	dGeomID Geom[1];  // geometries representing this body
}PhysObject;

class
{
	GENERATED_DESC

	methods(
		VMMATRIX(*GetTransform)(selfptr);
		VMMATRIX(*Tick)(selfptr);
	);
	struct CubeTex* pMesh;
	dWorldID World;
};