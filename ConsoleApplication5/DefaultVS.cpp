#include "IndexedTriangleList.h"
#include "DefaultVS.h"
#include "Class.h"

void virtual(Apply)(selfptr, VMVECTOR* _out, struct IndexedTriangleList* _in)
{
	for (int i = 0; i < _in->numVerts; i++)
	{
		_out[i] = VMVector3TransformCoord(VMLoadFloat3(&_in->vertices[i].pos), self->Transformation);
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