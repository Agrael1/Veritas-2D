#include "CubeTexDr.h"
#include "Camera.h"
#include "VActor.h"
#include "Class.h"

void virtual(Reset)(selfptr)
{
	self->Mesh->pos = (VMFLOAT3A) { 0.0f, 0.0f, 0.0f };
}
void virtual(Move)(selfptr, VMFLOAT3A dPosition)
{
	VMVECTOR R = VMLoadFloat3A(&dPosition);
	self->Mesh->orientation.y = self->ACamera->theta;
	R = VMVector3Transform(
		R,
		VMMatrixScale(VMMatrixRotationRollPitchYaw( 0.0f,self->ACamera->theta, 0.0f), self->movespeed));
	VMStoreFloat3A(&self->Mesh->pos, VMVectorAdd(R, VMLoadFloat3A(&self->Mesh->pos)));
}

VirtualTable{
	.Move = virtual(Move)
};
Constructor(selfptr, va_list *ap)
{
	account(self);
	assignMethodTable(self);
	self->Mesh = new(CubeDr);

	Reset_VActor(self);
	self->ACamera = new(Camera, &self->Mesh->pos);
	self->movespeed = 2.0f;
	return self;
}
Destructor(selfptr)
{
	delete(self->Mesh);
	delete(self->ACamera);
	return self;
}
ENDCLASSDESC