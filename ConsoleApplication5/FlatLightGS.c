#include "Color.h"
#include "Light.h"
#include "Standard.h"
#include "Icosphere.h"
#include "FlatLightGS.h"
#include "Class.h"

extern IAOut ia;
GSOut gs;

void virtual(Apply)(selfptr, void* v0, void* v1, void* v2)
{
	if (self->light)
	{
		Vertex_Icosphere* v0p = v0;
		VMVECTOR n = VMVector3Dot(self->light->LightCBuf.dir.v, VMVectorNegate(v0p->n));

		gs.color = Pick(BG_Dark_Magenta, (Byte)(n.m128_f32[0]*4.0f), self->light->LightCBuf.diffuse);
	}
	else
		gs.color = Pick(BG_Moon_White, 4, FG_Dark_Blue);
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Apply = virtual(Apply);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC