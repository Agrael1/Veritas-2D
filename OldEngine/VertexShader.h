#pragma once
#include "Codex.h"
#include "VSBase.h"

#define c_class VertexShader

class
{
	implements(VSBase);
};
interface
{
	implements(Bindable);
};
ComposeType;

inline String virtual(GenerateUID)(char* info)
{
	return make_string(info);
}
inline shared_ptr(Bindable)* virtual(Resolve)(char* info, void* pVS)
{
	__Resolve(info, pVS);
}
#ifndef VS_IMPL
#undef c_class
#endif // !VS_IMPL
