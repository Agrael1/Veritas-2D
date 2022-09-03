#pragma once
#include "Codex.h"
#include "Colormap.h"

#define c_class TextureBuffer

class
{
	ColorMap Texture;
};
interface
{
	implements(Bindable);
};
ComposeType;

HRESULT LoadTextureFromFileW(const char* const wFile, ColorMap* map);
inline String virtual(GenerateUID)(const char* fileName)
{
	return make_string(fileName);
}
inline shared_ptr(Bindable)* virtual(Resolve)(const char* fileName)
{
	__Resolve(fileName);
}
#ifndef TB_IMPL
#undef c_class
#endif // !TB_IMPL