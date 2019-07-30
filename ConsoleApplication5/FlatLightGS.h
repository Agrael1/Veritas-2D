#pragma once
#include "GSBase.h"
#include "Pipeline.h"
#include "EngineCommons.h"

#define c_class FlatLightGS

typedef struct
{
	CHAR_INFO color;
}GSOut;

class
{
	inherits(GSBase);
	struct DirectionalLight* light;
	VMMATRIX Proj;
};