#pragma once
#include "VeritasEngine.h"
#include "VLine.h"
#include "Light.h"
#include "Camera.h"
#include "EngineCommons.h"

#define c_class CubeDemo

class 
{
	inherits(VeritasEngine);
	
	struct Camera* pActiveCamera;
	struct VLine* pPl;
	struct DirectionalLight* pLight;

	struct Drawable* mesh;
	struct VActor* actor;
	struct Physics* physics;
	struct Model* model;
	
	float fTheta;
	bool bStop;
};