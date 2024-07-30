#include "Icosphere.h"
#include "DefaultVS.h"
#include "Class.h"

#undef c_class
#define c_class DefaultVS

void virtual(Apply)(void* self, void* _out, IndexedTriangleList* _in)
{
	account(self);
	VMVector3TransformStream(_out, this->VSOutSize, _in->vertices, _in->VSize, _in->numVerts, this->ModelViewProj);
	VMVector3TransformNormalStream((VMFLOAT3*)((Byte*)_out + sizeof(VMVECTOR)), this->VSOutSize,
		(VMFLOAT3*)((Byte*)_in->vertices + sizeof(VMVECTOR)), _in->VSize, _in->numVerts, this->ModelViewProj);
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	this->Apply = virtual(Apply);
	this->VSOutSize = sizeof(Vertex_Icosphere);
	self->ModelViewProj = VMMatrixIdentity();
	return self;
}
ENDCLASSDESCDD