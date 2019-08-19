#pragma once
#include "VeritasMath.h"
#include "EngineCommons.h"

#define TravelSpeed 12.0f
#define RotationSpeed 0.004f

#define c_class Camera

class
{
	methods(
		VMMATRIX(*GetViewMatrix)(selfptr);
		void(*Reset)(selfptr);
		void(*Rotate)(selfptr, float dx, float dy);
		void(*Translate)(selfptr, VMFLOAT3A translation);
	);
	VMFLOAT3A pos;
	float pitch;
	float yaw;
};