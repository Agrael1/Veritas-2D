#include "IndexedTriangleList.h"
#include "DefaultVS.h"
#include "Class.h"

void virtual(Apply)(selfptr, void* _out, struct IndexedTriangleList* _in)
{
	for (int i = 0; i < _in->numVerts; i++)
	{
		((Vertex_Icosphere*)_out)[i].pos.v = VMVector3TransformCoord(((Vertex_Icosphere*)_in->vertices)[i].pos.v, self->Transformation);
	}
}

constructMethodTable(
	.Apply = virtual(Apply)
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	self->Transformation = VMMatrixIdentity();
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC