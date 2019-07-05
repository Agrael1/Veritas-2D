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
#pragma endregion