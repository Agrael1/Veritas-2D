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
	VMFLOAT3A WorldPosition;
	struct CubeTex* Mesh;
	struct Camera* ACamera;
};