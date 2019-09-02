#pragma once
#include "Drawable.h"
#include "VeritasMath.h"
#include "BindableBase.h"
#include "EngineCommons.h"

#define c_class TransformBuffer

class
{
	inherits(Bindable);
	VMMATRIX ModelViewProjection;
	struct Drawable* parent;
};

inline char* virtual(GenerateUID)(struct Drawable* info)
{
	return (char*)stringOf(info->_class);
}
inline shared_ptr* virtual(Resolve)(struct Drawable* pParent, unsigned slot)
{
	__Resolve(pParent, pParent, slot)
}