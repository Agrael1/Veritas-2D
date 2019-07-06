#pragma once
#include "VTypes.h"
#include <assert.h>
#include <math.h>
#include "MathConstants.h"
#include "EngineCommons.h"

#define XM_PERMUTE_PS( v, c ) _mm_shuffle_ps( v, v, c )

inline bool XMScalarNearEqual
(
	float S1,
	float S2,
	float Epsilon
)
{
	float Delta = S1 - S2;
	return (fabsf(Delta) <= Epsilon);
}

#pragma region Vector
inline VMVECTOR __vectorcall VMVector3Transform
(
	FVMVECTOR V,
	FXMMATRIX M
)
{
	VMVECTOR vResult = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
	vResult = _mm_mul_ps(vResult, M.r[0]);
	VMVECTOR vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	vTemp = _mm_mul_ps(vTemp, M.r[1]);
	vResult = _mm_add_ps(vResult, vTemp);
	vTemp = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	vTemp = _mm_mul_ps(vTemp, M.r[2]);
	vResult = _mm_add_ps(vResult, vTemp);
	vResult = _mm_add_ps(vResult, M.r[3]);
	return vResult;
}
inline VMVECTOR __vectorcall VMLoadFloat3
(
	const VMFLOAT3* pSource
)
{
	assert(pSource);
	__m128 x = _mm_load_ss(&pSource->x);
	__m128 y = _mm_load_ss(&pSource->y);
	__m128 z = _mm_load_ss(&pSource->z);
	__m128 xy = _mm_unpacklo_ps(x, y);
	return _mm_movelh_ps(xy, z);
}
inline VMVECTOR __vectorcall VMVectorSet
(
	float x,
	float y,
	float z,
	float w
)
{
	return _mm_set_ps(w, z, y, x);
}
inline VMVECTOR __vectorcall VMVectorMultiply
(
	FVMVECTOR V1,
	FVMVECTOR V2
)
{
	return _mm_mul_ps(V1, V2);
}
#pragma endregion

#pragma region Matrix
inline VMMATRIX __vectorcall VMMatrixPerspectiveLH
(
	float ViewWidth,
	float ViewHeight,
	float NearZ,
	float FarZ
)
{
	assert(!XMScalarNearEqual(ViewWidth, 0.0f, 0.00001f));
	assert(!XMScalarNearEqual(ViewHeight, 0.0f, 0.00001f));
	assert(!XMScalarNearEqual(FarZ, NearZ, 0.00001f));

	VMMATRIX M;
	float TwoNearZ = NearZ + NearZ;
	float fRange = FarZ / (FarZ - NearZ);
	// Note: This is recorded on the stack
	VMVECTOR rMem = {
		TwoNearZ / ViewWidth,
		TwoNearZ / ViewHeight,
		fRange,
		-fRange * NearZ
	};
	// Copy from memory to SSE register
	VMVECTOR vValues = rMem;
	VMVECTOR vTemp = _mm_setzero_ps();
	// Copy x only
	vTemp = _mm_move_ss(vTemp, vValues);
	// TwoNearZ / ViewWidth,0,0,0
	M.r[0] = vTemp;
	// 0,TwoNearZ / ViewHeight,0,0
	vTemp = vValues;
	vTemp = _mm_and_ps(vTemp, g_XMMaskY.v);
	M.r[1] = vTemp;
	// x=fRange,y=-fRange * NearZ,0,1.0f
	vValues = _mm_shuffle_ps(vValues, g_XMIdentityR3.v, _MM_SHUFFLE(3, 2, 3, 2));
	// 0,0,fRange,1.0f
	vTemp = _mm_setzero_ps();
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(3, 0, 0, 0));
	M.r[2] = vTemp;
	// 0,0,-fRange * NearZ,0
	vTemp = _mm_shuffle_ps(vTemp, vValues, _MM_SHUFFLE(2, 1, 0, 0));
	M.r[3] = vTemp;

	return M;
}
inline VMMATRIX __vectorcall VMMatrixMultiply
(
	FXMMATRIX M1,
	CXMMATRIX M2
)
{
	VMMATRIX mResult;
	// Use vW to hold the original row
	VMVECTOR vW = M1.r[0];
	// Splat the component X,Y,Z then W
	VMVECTOR vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	VMVECTOR vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	VMVECTOR vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	// Perform the operation on the first row
	vX = _mm_mul_ps(vX, M2->r[0]);
	vY = _mm_mul_ps(vY, M2->r[1]);
	vZ = _mm_mul_ps(vZ, M2->r[2]);
	vW = _mm_mul_ps(vW, M2->r[3]);
	// Perform a binary add to reduce cumulative errors
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	mResult.r[0] = vX;
	// Repeat for the other 3 rows
	vW = M1.r[1];
	vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2->r[0]);
	vY = _mm_mul_ps(vY, M2->r[1]);
	vZ = _mm_mul_ps(vZ, M2->r[2]);
	vW = _mm_mul_ps(vW, M2->r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	mResult.r[1] = vX;
	vW = M1.r[2];
	vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2->r[0]);
	vY = _mm_mul_ps(vY, M2->r[1]);
	vZ = _mm_mul_ps(vZ, M2->r[2]);
	vW = _mm_mul_ps(vW, M2->r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	mResult.r[2] = vX;
	vW = M1.r[3];
	vX = XM_PERMUTE_PS(vW, _MM_SHUFFLE(0, 0, 0, 0));
	vY = XM_PERMUTE_PS(vW, _MM_SHUFFLE(1, 1, 1, 1));
	vZ = XM_PERMUTE_PS(vW, _MM_SHUFFLE(2, 2, 2, 2));
	vW = XM_PERMUTE_PS(vW, _MM_SHUFFLE(3, 3, 3, 3));
	vX = _mm_mul_ps(vX, M2->r[0]);
	vY = _mm_mul_ps(vY, M2->r[1]);
	vZ = _mm_mul_ps(vZ, M2->r[2]);
	vW = _mm_mul_ps(vW, M2->r[3]);
	vX = _mm_add_ps(vX, vZ);
	vY = _mm_add_ps(vY, vW);
	vX = _mm_add_ps(vX, vY);
	mResult.r[3] = vX;
	return mResult;
}
inline VMMATRIX __vectorcall VMMatrixTranslation
(
	float OffsetX,
	float OffsetY,
	float OffsetZ
)
{
	VMMATRIX M;
	M.r[0] = g_XMIdentityR0.v;
	M.r[1] = g_XMIdentityR1.v;
	M.r[2] = g_XMIdentityR2.v;
	M.r[3] = VMVectorSet(OffsetX, OffsetY, OffsetZ, 1.f);
	return M;
}
#pragma endregion

