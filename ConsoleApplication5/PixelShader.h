#pragma once
#include "Codex.h"
#include "PSBase.h"

#define c_class PixelShader

class
{
	implements(PSBase);
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
inline shared_ptr(Bindable)* virtual(Resolve)(char* info, void* pPS)
{
	__Resolve(info, pPS);
}
#ifndef PS_IMPL
#undef c_class
#endif // !PS_IMPL