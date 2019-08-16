#pragma once
#include "VSBase.h"
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class GouraudVST

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