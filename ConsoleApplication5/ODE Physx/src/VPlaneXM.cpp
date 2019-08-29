/************************************************************************
*                                                                       *
* Open Dynamics Engine, Copyright (C) 2019 Ilya "Agrael" Doroshenko.	*
* All rights reserved.  Email: agrael@live.ru							*
*                                                                       *
* This library is free software; you can redistribute it and/or         *
* modify it under the terms of EITHER:                                  *
*   (1) The GNU Lesser General Public License as published by the Free  *
*       Software Foundation; either version 2.1 of the License, or (at  *
*       your option) any later version. The text of the GNU Lesser      *
*       General Public License is included with this library in the     *
*       file LICENSE.TXT.                                               *
*   (2) The BSD-style license that is included with this library in     *
*       the file LICENSE-BSD.TXT.                                       *
*                                                                       *
* This library is distributed in the hope that it will be useful,       *
* but WITHOUT ANY WARRANTY; without even the implied warranty of        *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files    *
* LICENSE.TXT and LICENSE-BSD.TXT for more details.                     *
*                                                                       *
*************************************************************************/

/*
standard ODE geometry primitives: public API and pairwise collision functions.
the rule is that only the low level primitive collision functions should set
dContactGeom::g1 and dContactGeom::g2.

SIMD math instructions implementation
*/

#include <ode/collision.h>
#include <ode/common.h>
#include "VPlaneXM.h"

//****************************************************************************
// plane public API
VPlane::VPlane(dSpaceID space, float a, float b, float c, float d)noexcept 
	: dxGeom(space, 0)
{
	type = dPlaneClass;
	SetPlane(VMVectorSet(a, b, c, d));
}
VPlane::VPlane(dSpaceID space, VMVECTOR planeEq) noexcept
	: dxGeom(space, 0)
{
	type = dPlaneClass;
	SetPlane(planeEq);
}

void VPlane::computeAABB()noexcept
{
	aabb[0] = -dInfinity;
	aabb[1] = dInfinity;
	aabb[2] = -dInfinity;
	aabb[3] = dInfinity;
	aabb[4] = -dInfinity;
	aabb[5] = dInfinity;

	// Planes that have normal vectors aligned along an axis can use a
	// less comprehensive (half space) bounding box.

	switch (VMVector3EqualRaw(GetPlane(), VMVectorZero()))
	{
	case 0b110: 
	{
		// normal aligned with x-axis
		aabb[0] = (plane.x > 0.0f) ? -dInfinity : -plane.w;
		aabb[1] = (plane.x > 0.0f) ? plane.w : dInfinity;
		break;
	}
	case 0b101:
	{
		// normal aligned with x-axis
		aabb[2] = (plane.y > 0.0f) ? -dInfinity : -plane.w;
		aabb[3] = (plane.y > 0.0f) ? plane.w : dInfinity;
		break;
	}
	case 0b011:
	{
		// normal aligned with x-axis
		aabb[4] = (plane.z > 0.0f) ? -dInfinity : -plane.w;
		aabb[5] = (plane.z > 0.0f) ? plane.w : dInfinity;
		break;
	}
	}
}

// C accessors
dGeomID dCreateVPlane(dSpaceID space, VMVECTOR plane)
{
	return new VPlane(space, plane);
}
void dGeomVPlaneSetParams(dGeomID g, VMVECTOR plane)
{
	dUASSERT(g && g->type == dPlaneClass, "argument is not a plane");
	VPlane *p = reinterpret_cast<VPlane*>(g);
	p->SetPlane(plane);
	dGeomMoved(g);
}
VMVECTOR dGeomVPlaneGetParams(dGeomID g)
{
	dUASSERT(g && g->type == dPlaneClass, "argument not a plane");
	VPlane *p = reinterpret_cast<VPlane*>(g);
	return p->GetPlane();
}
VMVECTOR dGeomVPlanePointDepth(dGeomID g, VMVECTOR point3D)
{
	dUASSERT(g && g->type == dPlaneClass, "argument not a plane");

	VPlane *p = reinterpret_cast<VPlane*>(g);
	VMVECTOR plane = p->GetPlane();
	VMVECTOR res = VMVector3Dot(
		VMVectorAdd(VMVectorNegate(point3D), g_XMNegateW.v),
		plane);
	return res;
}
