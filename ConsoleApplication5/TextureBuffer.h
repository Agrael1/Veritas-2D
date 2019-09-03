#pragma once
#include "BindableBase.h"
#include "EngineCommons.h"

#define c_class TextureBuffer

class
{
	inherits(Bindable);
	void* Texture;
	unsigned slot;
};

inline char* virtual(GenerateUID)(char* info)
{
	return info;
}
inline shared_ptr virtual(Resolve)(char* info, void* pTex, unsigned slot)
{
	__Resolve(info, pTex, slot)
}