#include "Color.h"
#include "Light.h"
#include "Standard.h"
#include "Icosphere.h"
#include "FlatLightGS.h"
#include "Class.h"

extern IAOut ia;
GSOut gs;

void virtual(Apply)(void* self, void* v0, void* v1, void* v2)
{
	account(self);
	if (this->light)
	{
		Vertex_Icosphere* v0p = v0;
		VMVECTOR n = VMVector3Dot(this->light->Buf.dir, VMVector3Normalize(VMVectorNegate(v0p->n)));

		gs.color = Pick(BG_Ditry_Gray, (Byte)(n.m128_f32[0]*4.0f), this->light->Buf.diffuse);
	}
	else
		gs.color = Pick(BG_Moon_White, 4, FG_Dark_Blue);
}

Constructor(selfptr, va_list *ap)
{
	self->Apply = virtual(Apply);
	return self;
}
ENDCLASSDESCDD