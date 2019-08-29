#include "PhysicsAggregate.h"
#include "Class.h"

VirtualTable{
	0
};

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	self->World = dWorldCreate();		//Create World ODE
	self->Space = dSimpleSpaceCreate(0);
	self->contactgroup = dJointGroupCreate(0);

	dCreateVPlane(self->Space, g_XMIdentityR1.v);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC