#pragma once
#include "collision_kernel.h"
#include <VeritasMath.h>

class VPlane : public dxGeom 
{
public:
	VPlane(dSpaceID space, float a, float b, float c, float d)noexcept;
	VPlane(dSpaceID space, VMVECTOR planeEq)noexcept;
public:
	virtual void computeAABB()noexcept override;
public:
	void SetPlane(VMVECTOR planeEq)noexcept { VMStoreFloat4A(&plane, VMPlaneNormalizeEst(planeEq)); }
	VMVECTOR GetPlane()const noexcept { return VMLoadFloat4A(&plane); }
private:
	VMFLOAT4A plane;
};
