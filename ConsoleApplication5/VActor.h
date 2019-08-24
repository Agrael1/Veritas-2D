#pragma once
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class VActor

class
{
	GENERATED_DESC
	methods(
		void(*Move)(selfptr, VMFLOAT3A dPosition);
		VMMATRIX(*GetTransformVM)(const selfptr);
	);
	struct CubeTex* Mesh;
	struct Camera* ACamera;

	VMFLOAT3A WorldPosition;
	float movespeed;
};