#include "Icosphere.h"
#include "IndexedTriangleList.h"
#include "DefaultVS.h"
#include "Class.h"

#undef c_class
#define c_class DefaultVS

void virtual(Apply)(void* self, void* _out, struct IndexedTriangleList* _in)
{
	account(self);
	for (int i = 0; i < _in->numVerts; i++)
	{
		((Vertex_Icosphere*)_out)[i].pos.v = VMVector3TransformCoord(((Vertex_Icosphere*)_in->vertices)[i].pos.v, this->ModelViewProj);
	}
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Apply = virtual(Apply);
	self->ModelViewProj = VMMatrixIdentity();
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC