#pragma once
#include "VeritasEngine.h"
#include "VTypes.h"
#include "IndexedTriangleList.h"
#include "Pipeline.h"
#include "Camera.h"
#include "EngineCommons.h"

#define c_class CubeDemo

class 
{
	inherits(VeritasEngine);
	
	float fTheta;
	struct Camera* pCam;
	struct Pipeline* pPl;

	bool bStop;
	struct IndexedTriangleList model;
};