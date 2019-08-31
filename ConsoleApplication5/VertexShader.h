#pragma once
#include "BindableBase.h"
#include "EngineCommons.h"

#define c_class VertexShader

class
{
	inherits(Bindable);
	struct VSBase* pVertexShader;
};

inline char* virtual(GenerateUID)(char* info)
{
	return info;
}
inline shared_ptr* virtual(Resolve)(char* info, void* pVS, void* pLight)
{
	__Resolve(info, pVS, pLight)
}