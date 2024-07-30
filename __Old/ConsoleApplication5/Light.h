#pragma once
#include "BindableBase.h"
#include "VeritasMath.h"
#include "EngineCommons.h"

#define c_class DirectionalLight

class
{
	inherits(Bindable);
	struct LightCBuf
	{
		VMVECTOR dir;
		unsigned diffuse;
		unsigned ambientForce;
	}Buf;
};