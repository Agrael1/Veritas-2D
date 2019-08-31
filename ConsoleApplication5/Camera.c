#include "Camera.h"
#include "Class.h"
#include "Standard.h"

void _Rotate(selfptr, float dx, float dy)
{
	self->theta = wrap_angle(self->theta + dx*RotationSpeed);
	self->phi = (float)clamp(self->phi + dy*RotationSpeed, -XM_PIDIV2*0.99, XM_PIDIV2*0.99);
}
void _LookAround(selfptr, float dx, float dy)
{
	self->yaw = wrap_angle(self->yaw + dx*RotationSpeed);
	self->pitch = (float)clamp(self->pitch + dy*RotationSpeed, -XM_PIDIV2*0.99, XM_PIDIV2*0.99);
}
void _Translate(selfptr, VMFLOAT3A translation)
{
	const XMVECTORI32 mask = { UINT32_MAX, UINT32_MAX ,0 ,UINT32_MAX };
	VMVECTOR R = VMLoadFloat3A(&translation);
	R = VMVector3Transform(
		R,
		VMMatrixScale(VMMatrixRotationRollPitchYaw(self->pitch, self->yaw, 0.0f), TravelSpeed));
	R = VMVectorAdd(R, VMLoadFloat3A(&self->RelativePos));
	if (VMVector3GreaterMasked(R, g_XMZero.v, 0b0100))
	{
		R = VMVectorAnd(R, mask.v);
	}

	VMStoreFloat3A(&self->RelativePos, R);
}
VMMATRIX _GetViewMatrix(selfptr)
{
	const XMVECTORF32 forwardBase = { 0.0f,0.0f,1.0f,0.0f };

	FVMVECTOR lookVector = VMVector3Transform(forwardBase.v,
		VMMatrixRotationRollPitchYaw(self->phi, self->theta, 0.0f));
	VMVECTOR camPos;
	if (self->Anchor)
	{
		camPos = VMVectorAdd(
			VMVector3Transform(VMLoadFloat3A(&self->RelativePos), VMMatrixRotationRollPitchYaw(self->phi, self->theta, 0.0f)),
			VMLoadFloat3A(self->Anchor));
	}
	else
	{
		camPos = VMVector3Transform(VMLoadFloat3A(&self->RelativePos), VMMatrixRotationRollPitchYaw(self->phi, self->theta, 0.0f));
	}
	
	FVMVECTOR camTarget = VMVectorAdd(camPos, lookVector);
	VMMATRIX Res = VMMatrixLookAtLH(camPos, camTarget, g_XMIdentityR1.v);

	return Res;
}
void virtual(Reset)(selfptr)
{
	self->RelativePos = (VMFLOAT3A){ 0.0f, 0.5f, -3.998f };
	self->pitch = 0.0f;
	self->yaw = 0.0f;
}

constructMethodTable(
	.Reset = virtual(Reset),
	.GetViewMatrix = _GetViewMatrix,
	.Rotate = _Rotate,
	.LookAround = _LookAround,
	.Translate = _Translate
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	virtual(Reset)(self);
	self->Anchor = va_arg(*ap, VMFLOAT3A*);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC