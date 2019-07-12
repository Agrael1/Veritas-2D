#include "Camera.h"
#include "Class.h"

VMMATRIX _GetViewMatrix(const selfptr)
{
	FVMVECTOR pos = VMVector3Transform(
		VMVectorSet(0.0f, 0.0f, -self->r, 0.0f),
		VMMatrixRotationRollPitchYaw(self->phi, -self->theta, 0.0f)
	);

	FVMVECTOR up = VMVector3TransformCoord(
		VMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
		VMMatrixRotationRollPitchYaw(self->pitch, -self->yaw, self->roll));

	return VMMatrixLookAtLH(pos, VMVectorZero(),up);
}
void virtual(Reset)(selfptr)
{
	self->r = 20.0f;
	self->phi = 0.0f;
	self->theta = 0.0f;
	self->pitch = 0.0f;
	self->yaw = 0.0f;
	self->roll = 0.0f;
}

constructMethodTable(
	.Reset = virtual(Reset),
	.GetViewMatrix = _GetViewMatrix
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	virtual(Reset)(self);

	return self;
}
Destructor(selfptr)
{
	account(self);
	return this;
}
ENDCLASSDESC