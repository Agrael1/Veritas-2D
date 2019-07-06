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

#include "VMVector.h"

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
inline VMVECTOR __vectorcall VMQuaternionRotationRollPitchYawFromVector
(
	FVMVECTOR Angles // <Pitch, Yaw, Roll, 0>
)
{
	static const XMVECTORF32  Sign = { 1.0f, -1.0f, -1.0f, 1.0f };

	VMVECTOR HalfAngles = VMVectorMultiply(Angles, g_XMOneHalf.v);

	VMVECTOR SinAngles, CosAngles;
	VMVectorSinCos(&SinAngles, &CosAngles, HalfAngles);

	VMVECTOR P0 = VMVectorPermute(SinAngles, CosAngles,XM_PERMUTE_0X, XM_PERMUTE_1X, XM_PERMUTE_1X, XM_PERMUTE_1X);
	VMVECTOR Y0 = VMVectorPermute(SinAngles, CosAngles,XM_PERMUTE_1Y, XM_PERMUTE_0Y, XM_PERMUTE_1Y, XM_PERMUTE_1Y);
	VMVECTOR R0 = VMVectorPermute(SinAngles, CosAngles,XM_PERMUTE_1Z, XM_PERMUTE_1Z, XM_PERMUTE_0Z, XM_PERMUTE_1Z);
	VMVECTOR P1 = VMVectorPermute(CosAngles, SinAngles,XM_PERMUTE_0X, XM_PERMUTE_1X, XM_PERMUTE_1X, XM_PERMUTE_1X);
	VMVECTOR Y1 = VMVectorPermute(CosAngles, SinAngles,XM_PERMUTE_1Y, XM_PERMUTE_0Y, XM_PERMUTE_1Y, XM_PERMUTE_1Y);
	VMVECTOR R1 = VMVectorPermute(CosAngles, SinAngles,XM_PERMUTE_1Z, XM_PERMUTE_1Z, XM_PERMUTE_0Z, XM_PERMUTE_1Z);

	VMVECTOR Q1 = VMVectorMultiply(P1, Sign.v);
	VMVECTOR Q0 = VMVectorMultiply(P0, Y0);
	Q1 = VMVectorMultiply(Q1, Y1);
	Q0 = VMVectorMultiply(Q0, R0);
	VMVECTOR Q = VMVectorMultiplyAdd(Q1, R1, Q0);

	return Q;
}
inline VMMATRIX __vectorcall VMMatrixRotationQuaternion
(
	FVMVECTOR Quaternion
)
{
	static const XMVECTORF32  Constant1110 = { 1.0f, 1.0f, 1.0f, 0.0f };

	VMVECTOR Q0 = _mm_add_ps(Quaternion, Quaternion);
	VMVECTOR Q1 = _mm_mul_ps(Quaternion, Q0);

	VMVECTOR V0 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 0, 0, 1));
	V0 = _mm_and_ps(V0, g_XMMask3.v);
	VMVECTOR V1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(3, 1, 2, 2));
	V1 = _mm_and_ps(V1, g_XMMask3.v);
	VMVECTOR R0 = _mm_sub_ps(Constant1110.v, V0);
	R0 = _mm_sub_ps(R0, V1);

	V0 = XM_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 1, 0, 0));
	V1 = XM_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 2, 1, 2));
	V0 = _mm_mul_ps(V0, V1);

	V1 = XM_PERMUTE_PS(Quaternion, _MM_SHUFFLE(3, 3, 3, 3));
	VMVECTOR V2 = XM_PERMUTE_PS(Q0, _MM_SHUFFLE(3, 0, 2, 1));
	V1 = _mm_mul_ps(V1, V2);

	VMVECTOR R1 = _mm_add_ps(V0, V1);
	VMVECTOR R2 = _mm_sub_ps(V0, V1);

	V0 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(1, 0, 2, 1));
	V0 = XM_PERMUTE_PS(V0, _MM_SHUFFLE(1, 3, 2, 0));
	V1 = _mm_shuffle_ps(R1, R2, _MM_SHUFFLE(2, 2, 0, 0));
	V1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 0, 2, 0));

	Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(1, 0, 3, 0));
	Q1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 2, 0));

	VMMATRIX M;
	M.r[0] = Q1;

	Q1 = _mm_shuffle_ps(R0, V0, _MM_SHUFFLE(3, 2, 3, 1));
	Q1 = XM_PERMUTE_PS(Q1, _MM_SHUFFLE(1, 3, 0, 2));
	M.r[1] = Q1;

	Q1 = _mm_shuffle_ps(V1, R0, _MM_SHUFFLE(3, 2, 1, 0));
	M.r[2] = Q1;
	M.r[3] = g_XMIdentityR3.v;
	return M;
}
inline VMMATRIX __vectorcall VMMatrixRotationRollPitchYawFromVector
(
	FVMVECTOR Angles // <Pitch, Yaw, Roll, undefined>
)
{
	VMVECTOR Q = VMQuaternionRotationRollPitchYawFromVector(Angles);
	return VMMatrixRotationQuaternion(Q);
}
inline VMMATRIX __vectorcall VMMatrixRotationRollPitchYaw
(
	float Pitch,
	float Yaw,
	float Roll
)
{
	VMVECTOR Angles = VMVectorSet(Pitch, Yaw, Roll, 0.0f);
	return VMMatrixRotationRollPitchYawFromVector(Angles);
}
#pragma endregion

