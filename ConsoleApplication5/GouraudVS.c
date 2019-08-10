#include "LoaderTest.h"
#include "Light.h"
#include "Color.h"
#include "GouraudVS.h"
#include "Class.h"

virtual(cBuf)GVSCB;

void virtual(Apply)(void* self, void* _out, struct IndexedTriangleList* _in)
{
	account(self);

	for (int i = 0; i < _in->numVerts; i++)
	{
		((Vertex_LoaderTest*)_out)[i].pos.v = VMVector3TransformCoord(((Vertex_LoaderTest*)_in->vertices)[i].pos.v, this->ModelViewProj);
		VMVECTOR normal = VMVector3Normalize(VMVector3TransformNormal(((Vertex_LoaderTest*)_in->vertices)[i].n, this->ModelViewProj));
		((Vertex_LoaderTest*)_out)[i].n = VMVector3Dot(this->light->LightCBuf.dir.v, VMVectorNegate(normal));
	}
	GVSCB.diffuse = this->light->LightCBuf.diffuse;
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.VSOutSize = sizeof(Vertex_LoaderTest);
	base.Apply = virtual(Apply);
	self->ModelViewProj = VMMatrixIdentity();
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC