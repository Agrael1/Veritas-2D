#pragma once
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class VActor

class
{
	GENERATED_DESC
	methods(
		void(*Move)(selfptr, VMFLOAT3A dPosition);
	);
	struct CubeDr* Mesh;
	struct Camera* ACamera;

	float movespeed;
};