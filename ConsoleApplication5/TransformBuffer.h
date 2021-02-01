#pragma once
#include "Codex.h"
#include "Drawable.h"
#include "VeritasMath.h"

#define c_class TransformBuffer

class
{
	implements(Bindable);
	VMMATRIX ModelViewProjection;
	struct Drawable* parent;
};

ComposeType;

inline char* virtual(GenerateUID)(struct Drawable* info)
{
	return (char*)stringOf(info->_class);
}
inline shared_ptr virtual(Resolve)(struct Drawable* pParent, unsigned slot)
{
	__Resolve(pParent, pParent, slot)
}