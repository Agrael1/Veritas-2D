#pragma once
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class Camera

class
{
	methods(
		VMMATRIX(*GetViewMatrix)(const selfptr);
		void(*Reset)(selfptr);
	);

	float r;
	float phi;
	float theta;

	float pitch;
	float yaw;
	float roll;
};