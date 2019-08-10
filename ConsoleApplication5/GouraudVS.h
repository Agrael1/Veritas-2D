#pragma once
#include "VSBase.h"
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class GouraudVS

typedef struct
{
	DWord diffuse;
}virtual(cBuf);

class
{
	inherits(VSBase);
	struct DirectionalLight* light;
	VMMATRIX ModelViewProj;
};