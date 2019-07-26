#include "Color.h"
#include "Light.h"
#include "Class.h"

Constructor(selfptr, va_list *ap)
{
	self->LightCBuf.ambientForce = 1u;
	self->LightCBuf.diffuse = BG_Moon_White;
	self->LightCBuf.dir.c = (VMFLOAT4) { 0.0f, 0.0f, 1.0f };
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC