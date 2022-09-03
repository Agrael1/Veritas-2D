#pragma once
#include "Colormap.h"
#include "VDefs.h"

struct PSBase
{
	ColorMap TextureBuffer;
};
struct PSBase_vtbl
{
	struct _CHAR_INFO(*Apply)(void* self, void* _unIn);
};