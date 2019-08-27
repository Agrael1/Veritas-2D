#include "BoundingSphere.h"
#include "Class.h"

IntersectData _Intersect(selfptr, const struct BoundingSphere* other)
{
	float distance = self->radius + other->radius;
	FVMVECTOR radiusDistance = _mm_load_ps1(&distance);
	FVMVECTOR centerDistance = VMVector3LengthEst(VMVectorSubtract(
		VMLoadFloat3A(&self->Center),
		VMLoadFloat3A(&other->Center)));

	return (IntersectData){
		.distance = VMVectorGetX(VMVectorSubtract(centerDistance, radiusDistance)),
		.intersect = VMVector3LessOrEqual(centerDistance, radiusDistance)};
}

constructMethodTable(
	.Intersect = _Intersect
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	self->Center = va_arg(*ap, VMFLOAT3A);
	self->radius = (float)va_arg(*ap, double);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC