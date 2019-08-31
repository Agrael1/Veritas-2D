#include "CubeSceneTex.h"
#include "Light.h"
#include "Color.h"
#include "GouraudVST.h"
#include "Class.h"


virtual(cBuf)GVSCB;

void virtual(Apply)(void* self, void* _out, IndexedTriangleList* _in)
{
	account(self);
	VMVector3TransformStream(_out, base.VSOutSize, _in->vertices, _in->VSize, _in->numVerts, this->ModelViewProj);
	for (int i = 0; i < _in->numVerts; i++)
	{
		VMVECTOR normal = VMVector3TransformNormal(((Vertex_CubeTex*)_in->vertices)[i].n, this->ModelViewProj);
		((Vertex_CubeTex*)_out)[i].n = VMVector3Dot(this->light->LightCBuf.dir.v, VMVectorNegate(normal));
		((Vertex_CubeTex*)_out)[i].tc = ((Vertex_CubeTex*)_in->vertices)[i].tc;
	}
	GVSCB.diffuse = this->light->LightCBuf.diffuse;
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.VSOutSize = sizeof(Vertex_CubeTex);
	base.Apply = virtual(Apply);
	self->ModelViewProj = VMMatrixIdentity();
	self->light = va_arg(*ap, void*);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC