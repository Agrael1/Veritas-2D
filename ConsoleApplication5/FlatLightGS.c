#include "Color.h"
#include "Light.h"
#include "Standard.h"
#include "VeritasMath.h"
#include "FlatLightGS.h"
#include "Class.h"

extern IAOut ia;
GSOut gs;

void virtual(Apply)(selfptr, void* v0, void* v1, void* v2, struct IndexedTriangleList* trilist)
{
	if (self->light)
	{
		VMVECTOR rv0 = ((VMVECTOR*)trilist->vertices)[trilist->indices[ia.SV_PrimID * 3 + 0]];
		VMVECTOR rv1 = ((VMVECTOR*)trilist->vertices)[trilist->indices[ia.SV_PrimID * 3 + 1]];
		VMVECTOR rv2 = ((VMVECTOR*)trilist->vertices)[trilist->indices[ia.SV_PrimID * 3 + 2]];
		rv0 = VMVector3TransformCoord(rv0, self->Proj);
		rv1 = VMVector3TransformCoord(rv1, self->Proj);
		rv2 = VMVector3TransformCoord(rv2, self->Proj);


		VMVECTOR n = VMVector3Normalize(VMVector3Cross(VMVectorSubtract(rv1, rv0), VMVectorSubtract(rv2, rv0)));
		n = VMVector3Dot(self->light->LightCBuf.dir.v, VMVectorNegate(n));

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