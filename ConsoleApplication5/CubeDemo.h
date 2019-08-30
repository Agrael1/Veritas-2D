#pragma once
#include "VeritasEngine.h"
#include "VMathTypes.h"
#include "VLine.h"
#include "Light.h"
#include "Camera.h"
#include "EngineCommons.h"

#define c_class CubeDemo

class 
{
	inherits(VeritasEngine);
	
	float fTheta;
	struct Camera* pCam;
	struct VLine* pPl;
	struct Physics* pP;
	struct DirectionalLight* pLight;

	bool bStop;
	struct CubeTex* model;
	struct VActor* actor;
};