#pragma once
#include "BindableBase.h"
#include "EngineCommons.h"

#define c_class PixelShader

class
{
	inherits(Bindable);
	struct PSBase* pPixelShader;
};

inline char* virtual(GenerateUID)(char* info)
{
	return info;
}
inline shared_ptr virtual(Resolve)(char* info, void* pPS)
{
	__Resolve(info, pPS)
}