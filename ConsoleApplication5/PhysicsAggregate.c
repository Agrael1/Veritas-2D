#include "CubeSceneTex.h"
#include "PhysicsAggregate.h"
#include "Class.h"

PhysObject Object;
dSpaceID Space;
dJointGroupID contactgroup;

VMMATRIX GetTransform(selfptr)
{
	int type = dGeomGetClass(Object.Geom[0]);
	if (type == dBoxClass) {

		const dReal * SPos = dGeomGetPosition(Object.Geom[0]);
		const dReal * SRot = dGeomGetRotation(Object.Geom[0]);

		float spos[3] = { SPos[0], SPos[1], SPos[2] };
		float srot[12] = { SRot[0], SRot[1], SRot[2], SRot[3], SRot[4], SRot[5], SRot[6], SRot[7], SRot[8], SRot[9], SRot[10], SRot[11] };
		int type = dGeomGetClass(Object.Geom[0]);

		//VMMATRIX RM = VMLoadFloat3x3(&srot);
		VMMATRIX RM = self->pMesh->_base.method->GetTransformXM(self->pMesh);
		VMMATRIX T = VMMatrixTranslation((float)spos[0], (float)spos[1], (float)spos[2]);
		VMMATRIX res = VMMatrixMultiply(T, &RM);
		return res;
	}
	else
	{
		return VMMatrixIdentity();
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
			dJointID c = dJointCreateContact(this->World, contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}
VMMATRIX Tick(selfptr)
{
	dSpaceCollide(Space, self, &nearCallback);
	dWorldQuickStep(self->World, 0.05f);
	dJointGroupEmpty(contactgroup);

	VMMATRIX M = GetTransform(self);
	return M;
}

VirtualTable{
	.GetTransform = GetTransform,
	.Tick = Tick
};
Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);

	dInitODE2(0);
	self->World = dWorldCreate();		//Create World ODE
	Space = dSimpleSpaceCreate(0);
	contactgroup = dJointGroupCreate(0);


	dCreatePlane(Space, 0,1,0,0);
	//dCreateVPlane(Space, g_XMIdentityR1.v);
	dWorldSetGravity(self->World, 0.0f, -0.1f, 0.0f);

	dWorldSetERP(self->World, 0.2f);
	dWorldSetCFM(self->World, 1e-5f);

	dWorldSetContactMaxCorrectingVel(self->World, 0.9f);
	dWorldSetContactSurfaceLayer(self->World, 0.001f);
	dWorldSetAutoDisableFlag(self->World, 1);
	//----------------------------------------------------
	
	dMass m;
	float sides[3];

	Object.Body = dBodyCreate(self->World);
	dBodySetPosition(Object.Body, 0, 2, 0);

	sides[0] = 1.0f;
	sides[1] = 1.0f;
	sides[2] = 1.0f;
	dMassSetBox(&m, 0.5f, 1.0f, sides[1], sides[2]);
	Object.Geom[0] = dCreateBox(Space, sides[0], sides[1], sides[2]);
	dGeomSetBody(Object.Geom[0], Object.Body);

	dMatrix3 R;
	dRSetIdentity(R);
	dBodySetRotation(Object.Body, R);
	dBodySetMass(Object.Body, &m);

	SVMVECTOR tempVect = { 0.0, 0.0, 0.0 };
	dBodySetLinearVel(Object.Body, tempVect.c.x, tempVect.c.y, tempVect.c.z);

	self->pMesh = new(CubeTex,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f);
	return self;
}
Destructor(selfptr)
{
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(Space);
	dWorldDestroy(self->World);
	dCloseODE();
	return self;
}
ENDCLASSDESC