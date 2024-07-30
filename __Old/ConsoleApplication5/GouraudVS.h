#pragma once
#include "VSBase.h"
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class GouraudVS

typedef struct
{
	unsigned diffuse;
}virtual(cBuf);

class
{
	extends(VSBase);
	struct DirectionalLight* light;
	VMMATRIX ModelViewProj;
};