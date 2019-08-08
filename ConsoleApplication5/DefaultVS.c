#include "Icosphere.h"
#include "DefaultVS.h"
#include "Class.h"

#undef c_class
#define c_class DefaultVS

void virtual(Apply)(void* self, void* _out, struct IndexedTriangleList* _in)
{
	account(self);
	VMVector3TransformStream(_out, base.VSOutSize, _in->vertices, _in->VSize, _in->numVerts, this->ModelViewProj);
	VMVector3TransformNormalStream((Byte*)_out + sizeof(VMVECTOR), base.VSOutSize,
		(Byte*)_in->vertices + sizeof(VMVECTOR), _in->VSize, _in->numVerts, this->ModelViewProj);
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