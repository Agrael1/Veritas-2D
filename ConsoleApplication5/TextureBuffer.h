#pragma once
#include "BindableBase.h"
#include "EngineCommons.h"
#include "Colormap.h"

#define c_class TextureBuffer

class
{
	inherits(Bindable);
	ColorMap Texture;
	unsigned slot;
};

inline const char* virtual(GenerateUID)(const char* info)
{
	return info;
}
inline shared_ptr virtual(Resolve)(const char* fileName, unsigned slot)
{
	__Resolve(fileName, fileName, slot);
}