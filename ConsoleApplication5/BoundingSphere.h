#pragma once
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class BoundingSphere

typedef struct IntersectData
{
	float distance;
	bool intersect;
}IntersectData;

class
{
	GENERATED_DESC

	methods(
		IntersectData(*Intersect)(selfptr, const struct BoundingSphere* other);
	);
	VMFLOAT3A Center;
	float radius;
};