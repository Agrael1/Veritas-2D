#include "CubeSceneTex.h"
#include "Light.h"
#include "GouraudVST.h"
#include "Class.h"


virtual(cBuf)GVSCB;

void virtual(Apply)(void* self, void* _out, IndexedTriangleList* _in)
{
	account(self);
	VMMATRIX ModelViewProj = VMLoadFloat4x4A(base.ConstantBuffer[1]);
	struct LightCBuf* light = base.ConstantBuffer[0];

	VMVector3TransformStream(_out, base.VSOutSize, _in->vertices, _in->VSize, _in->numVerts, ModelViewProj);
	for (int i = 0; i < _in->numVerts; i++)
	{
		VMVECTOR normal = VMVector3TransformNormal(((Vertex_CubeTex*)_in->vertices)[i].n, ModelViewProj);
		((Vertex_CubeTex*)_out)[i].n = VMVector3Dot(light->dir, VMVectorNegate(normal));
		((Vertex_CubeTex*)_out)[i].tc = ((Vertex_CubeTex*)_in->vertices)[i].tc;
	}
	GVSCB.diffuse = light->diffuse;
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.VSOutSize = sizeof(Vertex_CubeTex);
	base.Apply = virtual(Apply);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC