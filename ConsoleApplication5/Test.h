#pragma once
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class TestObject

; class
{
	GENERATED_DESC
	methods(
		void(*Update)(selfptr, float dt);
		VMMATRIX(*GetTransformXM)(const selfptr);
	);

	// Positioning
	float r;
	float roll;
	float pitch;
	float yaw;
	float theta;
	float phi;
	float chi;
	// Speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};