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
	self->Bind = virtual(Bind);
	self->parent = va_arg(*ap, void*);
	return self;
}
ENDCLASSDESCDD