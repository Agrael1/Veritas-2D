#pragma once
#include "VeritasMath.h"
#include "EngineCommons.h"

#define TravelSpeed 0.2f
#define RotationSpeed 0.2f

#define c_class Camera

class
{
	GENERATED_DESC
	methods(
		VMMATRIX(*GetViewMatrix)(selfptr);
		void(*Reset)(selfptr);
		void(*Rotate)(selfptr, float dx, float dy);
		void(*LookAround)(selfptr, float dx, float dy);
		void(*Translate)(selfptr, VMFLOAT3A translation);
	);
	
	VMFLOAT3A* Anchor;
	VMFLOAT3A RelativePos;
	
	float pitch;
	float yaw;

	float phi;
	float theta;
};