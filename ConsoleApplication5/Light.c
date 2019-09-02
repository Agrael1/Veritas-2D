#include "Color.h"
#include "VSBase.h"
#include "VLine.h"
#include "Light.h"
#include "Class.h"

void virtual(Bind)(selfptr, struct VLine* gfx)
{
	gfx->VS.ConstantBuffer[0] = &self->Buf;
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Bind = virtual(Bind);
	self->Buf.ambientForce = 4u;
	self->Buf.diffuse = BG_Moon_White;
	self->Buf.dir = VMLoadFloat3A(&(VMFLOAT3A){ 0.0f, 0.0f, 1.0f });
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC