#pragma once
#include "../Vlib/include/RuntimeClass.h"
#include "VeritasMath.h"

#define TravelSpeed 0.2f
#define RotationSpeed 0.2f

#define c_class Camera
typedef struct Camera Camera;

void Constructor(selfptr, VMFLOAT3A* Anchor);

struct Camera
{	
	VMFLOAT3A* Anchor;
	VMFLOAT3A RelativePos;
	
	float pitch;
	float yaw;

	float phi;
	float theta;
};

//API
VMMATRIX CamGetViewMatrix(const selfptr);
void CamReset(selfptr);
void CamRotate(selfptr, float dx, float dy);
void CamLookAround(selfptr, float dx, float dy);
void CamTranslate(selfptr, VMFLOAT3A translation);