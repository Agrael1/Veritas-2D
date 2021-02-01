#include "VeritasMath.h"
#include "Test.h"
#include "Class.h"

VMMATRIX virtual(GetTransformXM)(const selfptr)
{
	VMMATRIX R1 = VMMatrixRotationRollPitchYaw(self->pitch, self->yaw, self->roll);
	VMMATRIX T = VMMatrixTranslation(self->r, 0.0f, 0.0f);
	VMMATRIX R2 = VMMatrixRotationRollPitchYaw(self->theta, self->phi, self->chi);

	VMMATRIX res = VMMatrixMultiply(
		VMMatrixMultiply(
			R1, &T), &R2);

	return res;
}
void virtual(Update)(selfptr, float dt)
{
	self->roll += self->droll * dt;
	self->pitch += self->dpitch * dt;
	self->yaw += self->dyaw * dt;
	self->theta += self->dtheta * dt;
	self->phi += self->dphi * dt;
	self->chi += self->dchi * dt;
}

constructMethodTable(
	.Update = virtual(Update),
	.GetTransformXM = virtual(GetTransformXM)
);

Constructor(selfptr, va_list *ap)
{
	InitializeVtable(self);

	self->roll = 0.0f;
	self->pitch = 0.0f;
	self->yaw = 0.0f;

	self->r = (float)va_arg(*ap, double);
	self->droll = (float)va_arg(*ap, double);
	self->dpitch = (float)va_arg(*ap, double);
	self->dyaw = (float)va_arg(*ap, double);
	self->dphi = (float)va_arg(*ap, double);
	self->dtheta = (float)va_arg(*ap, double);
	self->dchi = (float)va_arg(*ap, double);
	self->chi = (float)va_arg(*ap, double);
	self->theta = (float)va_arg(*ap, double);
	self->phi = (float)va_arg(*ap, double);

	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC