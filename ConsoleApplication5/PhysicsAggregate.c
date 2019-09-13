#include "CubeTexDr.h"
#include "Drawable.h"
#include "PhysicsAggregate.h"
#include "Class.h"

#pragma warning(disable:4133)

void SetTransform(selfptr)
{
	int type = dGeomGetClass(self->Object.Geom[0]);
	if (type == dBoxClass) {

		const dReal * SPos = dGeomGetPosition(self->Object.Geom[0]);
		const dReal * SRot = dGeomGetRotation(self->Object.Geom[0]);

		float spos[3] = { SPos[0], SPos[1], SPos[2] };
		float srot[12] = { SRot[0], SRot[1], SRot[2], SRot[3], SRot[4], SRot[5], SRot[6], SRot[7], SRot[8], SRot[9], SRot[10], SRot[11] };
		int type = dGeomGetClass(self->Object.Geom[0]);

		VMStoreFloat3A(&((struct CubeDr*)self->pMesh)->pos, VMLoadFloat3(SPos));
	}
}
void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	account(data);

	if (dGeomIsSpace(o1) || dGeomIsSpace(o2))
	{
		fprintf(stderr, "testing space %p %p\n", (void*)o1, (void*)o2);
		// colliding a space with something
		dSpaceCollide2(o1, o2, data, &nearCallback);
		// Note we do not want to test intersections within a space,
		// only between spaces.
		return;
	}

	int i;
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	dContact contact[2]; 

	for (i = 0; i < 2; i++)
	{
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = dInfinity;
		contact[i].surface.mu2 = 0.0f;
		contact[i].surface.bounce = 0.01f;
		contact[i].surface.bounce_vel = 0.1f;
		contact[i].surface.soft_cfm = 0.01f;
	}

	int numc = 0;
	if (numc = dCollide(o1, o2, 2, &contact[0].geom, sizeof(dContact)))
	{
		for (i = 0; i < numc; i++)
		{
			dJointID c = dJointCreateContact(this->World, this->contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}
void Tick(selfptr)
{
	dSpaceCollide(self->Space, self, &nearCallback);
	dWorldQuickStep(self->World, 0.05f);
	dJointGroupEmpty(self->contactgroup);

	SetTransform(self);
}

VirtualTable{
	.SetTransform = SetTransform,
	.Tick = Tick
};
Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);

	dInitODE2(0);
	self->World = dWorldCreate();		//Create World ODE
	self->Space = dSimpleSpaceCreate(0);
	self->contactgroup = dJointGroupCreate(0);


	dCreatePlane(self->Space, 0,1,0,0);
	//dCreateVPlane(Space, g_XMIdentityR1.v);
	dWorldSetGravity(self->World, 0.0f, -1.0f, 0.0f);

	dWorldSetERP(self->World, 0.2f);
	dWorldSetCFM(self->World, 1e-5f);

	dWorldSetContactMaxCorrectingVel(self->World, 0.9f);
	dWorldSetContactSurfaceLayer(self->World, 0.001f);
	dWorldSetAutoDisableFlag(self->World, 1);
	//----------------------------------------------------
	
	dMass m;
	float sides[3];

	self->Object.Body = dBodyCreate(self->World);
	dBodySetPosition(self->Object.Body, 0, 2, 0);

	sides[0] = 1.0f;
	sides[1] = 1.0f;
	sides[2] = 1.0f;
	dMassSetBox(&m, 0.5f, 1.0f, sides[1], sides[2]);
	self->Object.Geom[0] = dCreateBox(self->Space, sides[0], sides[1], sides[2]);
	dGeomSetBody(self->Object.Geom[0], self->Object.Body);

	dMatrix3 R;
	dRSetIdentity(R);
	dBodySetRotation(self->Object.Body, R);
	dBodySetMass(self->Object.Body, &m);

	SVMVECTOR tempVect = { 0.0f, 0.2f, 0.0f };
	dBodySetLinearVel(self->Object.Body, tempVect.c.x, tempVect.c.y, tempVect.c.z);

	self->pMesh = new(CubeDr);
	return self;
}
Destructor(selfptr)
{
	dJointGroupDestroy(self->contactgroup);
	dSpaceDestroy(self->Space);
	dWorldDestroy(self->World);
	dCloseODE();
	return self;
}
ENDCLASSDESC