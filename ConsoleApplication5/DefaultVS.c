#include "Icosphere.h"
#include "DefaultVS.h"
#include "Class.h"

#undef c_class
#define c_class DefaultVS

void virtual(Apply)(void* self, void* _out, struct IndexedTriangleList* _in)
{
	account(self);

	for (int i = 0; i < _in->numVerts; i++)
	{
		((Vertex_Icosphere*)_out)[i].pos.v = VMVector3Transform(((Vertex_Icosphere*)_in->vertices)[i].pos.v, this->ModelViewProj);
		((Vertex_Icosphere*)_out)[i].n = VMVector3Normalize(VMVector3TransformNormal(((Vertex_Icosphere*)_in->vertices)[i].n, this->ModelViewProj));
	}
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Apply = virtual(Apply);
	base.VSOutSize = sizeof(Vertex_Icosphere);
	self->ModelViewProj = VMMatrixIdentity();
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC