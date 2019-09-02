#include "LoaderTest.h"
#include "Light.h"
#include "Color.h"
#include "GouraudVS.h"
#include "Class.h"


virtual(cBuf)GVSCB;

void virtual(Apply)(void* self, void* _out, IndexedTriangleList* _in)
{
	account(self);
	struct LightCBuf* light = base.ConstantBuffer[1];
	VMVector3TransformStream(_out, base.VSOutSize, _in->vertices, _in->VSize, _in->numVerts, this->ModelViewProj);

	for (int i = 0; i < _in->numVerts; i++)
	{
		VMVECTOR normal = VMVector3Normalize(VMVector3TransformNormal(((Vertex_LoaderTest*)_in->vertices)[i].n, this->ModelViewProj));
		((Vertex_LoaderTest*)_out)[i].n = VMVector3Dot(light->dir, VMVectorNegate(normal));
	}
	GVSCB.diffuse = light->diffuse;
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