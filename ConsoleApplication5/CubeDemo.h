#pragma once
#include "VeritasEngine.h"
#include "VTypes.h"
#include "IndexedTriangleList.h"
#include "Camera.h"
#include "EngineCommons.h"

#define c_class CubeDemo

class 
{
	inherits(VeritasEngine);
	
	float fTheta;

	VMVECTOR ScreenOffset;
	VMVECTOR ScreenResolution;
	struct Camera* pCam;


	bool bStop;
	struct IndexedTriangleList model;
};