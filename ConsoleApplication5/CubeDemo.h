#pragma once
#include "VeritasEngine.h"
#include "VTypes.h"
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
	struct DirectionalLight* pLight;

	bool bStop;
	struct LoaderTest* model;
};