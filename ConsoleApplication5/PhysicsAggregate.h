#pragma once
#include "VeritasMath.h"
#include <ode\ode.h>
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
		int x;
	);

	VMFLOAT3X3 GeomMatrix;
	PhysObject Object;
	dWorldID World;
	dSpaceID Space;
	dJointGroupID contactgroup;
};