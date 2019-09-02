#include "VSBase.h"
#include "TransformBuffer.h"
#include "Class.h"

void virtual(Bind)(void* self, struct VLine* gfx)
{
	account(self);
	this->ModelViewProjection = VMMatrixMultiply(VMMatrixMultiply(
		this->parent->method->GetTransformXM(this->parent),
		&gfx->camera),&gfx->projection);

	gfx->VS.ConstantBuffer[1] = &this->ModelViewProjection;
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Bind = virtual(Bind);
	this->parent = va_arg(*ap, void*);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC