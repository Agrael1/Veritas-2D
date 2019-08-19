#include "Camera.h"
#include "Class.h"
#include "Standard.h"

void _Rotate(selfptr, float dx, float dy)
{
	self->yaw = wrap_angle(self->yaw + dx*RotationSpeed);
	self->pitch = (float)clamp(self->pitch + dy*RotationSpeed, -XM_PIDIV2*0.99, XM_PIDIV2*0.99);
}
void _Translate(selfptr, VMFLOAT3A translation)
{
	VMVECTOR R = VMLoadFloat3A(&translation);
	R = VMVector3Transform(
		R,
		VMMatrixScale(VMMatrixRotationRollPitchYaw(self->pitch, self->yaw, 0.0f), TravelSpeed));
	_mm_store_ps(&self->pos.x, VMVectorAdd(R, _mm_load_ps(&self->pos.x)));
}
VMMATRIX _GetViewMatrix(selfptr)
{
	const XMVECTORF32 forwardBase = { 0.0f,0.0f,1.0f,0.0f };

	FVMVECTOR lookVector = VMVector3Transform(forwardBase.v,
		VMMatrixRotationRollPitchYaw(self->pitch, self->yaw, 0.0f));

	FVMVECTOR camPos = VMLoadFloat3A(&self->pos);
	FVMVECTOR camTarget = VMVectorAdd(camPos, lookVector);
	VMMATRIX Res = VMMatrixLookAtLH(camPos, camTarget, g_XMIdentityR1.v);
	return Res;
}
void virtual(Reset)(selfptr)
{
	self->pos = (VMFLOAT3A){ 0.0f, 0.5f, -3.998f };
	self->pitch = 0.0f;
	self->yaw = 0.0f;
}

constructMethodTable(
	.Reset = virtual(Reset),
	.GetViewMatrix = _GetViewMatrix,
	.Rotate = _Rotate,
	.Translate = _Translate
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