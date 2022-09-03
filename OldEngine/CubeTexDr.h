#pragma once
#include "Drawable.h"
#include "EngineCommons.h"

#define c_class CubeDr

typedef struct
{
	SVMVECTOR pos;
	VMVECTOR n;
	SVMVECTOR tc;
}virtual(Vertex);

class
{
	inherits(Drawable);
	VMFLOAT3A pos;
	VMFLOAT3A orientation;
};