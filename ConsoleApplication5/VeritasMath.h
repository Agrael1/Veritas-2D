#pragma once
#include "VTypes.h"
#include <assert.h>
#include <math.h>
#include "MathConstants.h"
#include "EngineCommons.h"

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

#include "VMVector3.h"

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
inline VMMATRIX __vectorcall VMMatrixTranspose
(
	FXMMATRIX M
)
{
	// x.x,x.y,y.x,y.y
	VMVECTOR vTemp1 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(1, 0, 1, 0));
	// x.z,x.w,y.z,y.w
	VMVECTOR vTemp3 = _mm_shuffle_ps(M.r[0], M.r[1], _MM_SHUFFLE(3, 2, 3, 2));
	// z.x,z.y,w.x,w.y
	VMVECTOR vTemp2 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(1, 0, 1, 0));
	// z.z,z.w,w.z,w.w
	VMVECTOR vTemp4 = _mm_shuffle_ps(M.r[2], M.r[3], _MM_SHUFFLE(3, 2, 3, 2));
	VMMATRIX mResult;

	// x.x,y.x,z.x,w.x
	mResult.r[0] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
	// x.y,y.y,z.y,w.y
	mResult.r[1] = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
	// x.z,y.z,z.z,w.z
	mResult.r[2] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
	// x.w,y.w,z.w,w.w
	mResult.r[3] = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
	return mResult;
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
inline VMMATRIX __vectorcall VMMatrixLookToLH
(
	FVMVECTOR EyePosition,
	FVMVECTOR EyeDirection,
	FVMVECTOR UpDirection
)
{
	assert(!VMVector3Equal(EyeDirection, VMVectorZero()));
	assert(!VMVector3IsInfinite(EyeDirection));
	assert(!VMVector3Equal(UpDirection, VMVectorZero()));
	assert(!VMVector3IsInfinite(UpDirection));

	VMVECTOR R2 = VMVector3Normalize(EyeDirection);

	VMVECTOR R0 = VMVector3Cross(UpDirection, R2);
	R0 = VMVector3Normalize(R0);

	VMVECTOR R1 = VMVector3Cross(R2, R0);

	VMVECTOR NegEyePosition = VMVectorNegate(EyePosition);
	
	VMVECTOR D0 = VMVector3Dot(R0, NegEyePosition);
	VMVECTOR D1 = VMVector3Dot(R1, NegEyePosition);
	VMVECTOR D2 = VMVector3Dot(R2, NegEyePosition);
	
	VMMATRIX M;
	M.r[0] = VMVectorSelect(D0, R0, g_XMSelect1110.v);
	M.r[1] = VMVectorSelect(D1, R1, g_XMSelect1110.v);
	M.r[2] = VMVectorSelect(D2, R2, g_XMSelect1110.v);
	M.r[3] = g_XMIdentityR3.v;

	M = VMMatrixTranspose(M);

	return M;
}
inline VMMATRIX __vectorcall VMMatrixLookAtLH
(
	FVMVECTOR EyePosition,
	FVMVECTOR FocusPosition,
	FVMVECTOR UpDirection
)
{
	VMVECTOR EyeDirection = VMVectorSubtract(FocusPosition, EyePosition);
	return VMMatrixLookToLH(EyePosition, EyeDirection, UpDirection);
}
inline VMMATRIX __vectorcall VMMatrixIdentity()
{
	VMMATRIX M;
	M.r[0] = g_XMIdentityR0.v;
	M.r[1] = g_XMIdentityR1.v;
	M.r[2] = g_XMIdentityR2.v;
	M.r[3] = g_XMIdentityR3.v;
	return M;
}
inline VMMATRIX __vectorcall VMMatrixInverse
(
	VMVECTOR* pDeterminant,
	FXMMATRIX  M
)
{

	VMMATRIX MT = VMMatrixTranspose(M);
	VMVECTOR V00 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 1, 0, 0));
	VMVECTOR V10 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(3, 2, 3, 2));
	VMVECTOR V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 1, 0, 0));
	VMVECTOR V11 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(3, 2, 3, 2));
	VMVECTOR V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(2, 0, 2, 0));
	VMVECTOR V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(3, 1, 3, 1));
	
	VMVECTOR D0 = _mm_mul_ps(V00, V10);
	VMVECTOR D1 = _mm_mul_ps(V01, V11);
	VMVECTOR D2 = _mm_mul_ps(V02, V12);

	V00 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(3, 2, 3, 2));
	V10 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 1, 0, 0));
	V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(3, 2, 3, 2));
	V11 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 1, 0, 0));
	V02 = _mm_shuffle_ps(MT.r[2], MT.r[0], _MM_SHUFFLE(3, 1, 3, 1));
	V12 = _mm_shuffle_ps(MT.r[3], MT.r[1], _MM_SHUFFLE(2, 0, 2, 0));

	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	D0 = _mm_sub_ps(D0, V00);
	D1 = _mm_sub_ps(D1, V01);
	D2 = _mm_sub_ps(D2, V02);
	// V11 = D0Y,D0W,D2Y,D2Y
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 1, 3, 1));
	V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(1, 0, 2, 1));
	V10 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(0, 3, 0, 2));
	V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(0, 1, 0, 2));
	V11 = _mm_shuffle_ps(V11, D0, _MM_SHUFFLE(2, 1, 2, 1));
	// V13 = D1Y,D1W,D2W,D2W
	VMVECTOR V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 3, 3, 1));
	V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(1, 0, 2, 1));
	V12 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(0, 3, 0, 2));
	VMVECTOR V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(0, 1, 0, 2));
	V13 = _mm_shuffle_ps(V13, D1, _MM_SHUFFLE(2, 1, 2, 1));

	VMVECTOR C0 = _mm_mul_ps(V00, V10);
	VMVECTOR C2 = _mm_mul_ps(V01, V11);
	VMVECTOR C4 = _mm_mul_ps(V02, V12);
	VMVECTOR C6 = _mm_mul_ps(V03, V13);

	// V11 = D0X,D0Y,D2X,D2X
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(0, 0, 1, 0));
	V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(2, 1, 3, 2));
	V10 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(2, 1, 0, 3));
	V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(1, 3, 2, 3));
	V11 = _mm_shuffle_ps(D0, V11, _MM_SHUFFLE(0, 2, 1, 2));
	// V13 = D1X,D1Y,D2Z,D2Z
	V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(2, 2, 1, 0));
	V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(2, 1, 3, 2));
	V12 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(2, 1, 0, 3));
	V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(1, 3, 2, 3));
	V13 = _mm_shuffle_ps(D1, V13, _MM_SHUFFLE(0, 2, 1, 2));

	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	V03 = _mm_mul_ps(V03, V13);
	C0 = _mm_sub_ps(C0, V00);
	C2 = _mm_sub_ps(C2, V01);
	C4 = _mm_sub_ps(C4, V02);
	C6 = _mm_sub_ps(C6, V03);

	V00 = XM_PERMUTE_PS(MT.r[1], _MM_SHUFFLE(0, 3, 0, 3));
	// V10 = D0Z,D0Z,D2X,D2Y
	V10 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 2, 2));
	V10 = XM_PERMUTE_PS(V10, _MM_SHUFFLE(0, 2, 3, 0));
	V01 = XM_PERMUTE_PS(MT.r[0], _MM_SHUFFLE(2, 0, 3, 1));
	// V11 = D0X,D0W,D2X,D2Y
	V11 = _mm_shuffle_ps(D0, D2, _MM_SHUFFLE(1, 0, 3, 0));
	V11 = XM_PERMUTE_PS(V11, _MM_SHUFFLE(2, 1, 0, 3));
	V02 = XM_PERMUTE_PS(MT.r[3], _MM_SHUFFLE(0, 3, 0, 3));
	// V12 = D1Z,D1Z,D2Z,D2W
	V12 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 2, 2));
	V12 = XM_PERMUTE_PS(V12, _MM_SHUFFLE(0, 2, 3, 0));
	V03 = XM_PERMUTE_PS(MT.r[2], _MM_SHUFFLE(2, 0, 3, 1));
	// V13 = D1X,D1W,D2Z,D2W
	V13 = _mm_shuffle_ps(D1, D2, _MM_SHUFFLE(3, 2, 3, 0));
	V13 = XM_PERMUTE_PS(V13, _MM_SHUFFLE(2, 1, 0, 3));

	V00 = _mm_mul_ps(V00, V10);
	V01 = _mm_mul_ps(V01, V11);
	V02 = _mm_mul_ps(V02, V12);
	V03 = _mm_mul_ps(V03, V13);
	VMVECTOR C1 = _mm_sub_ps(C0, V00);
	C0 = _mm_add_ps(C0, V00);
	VMVECTOR C3 = _mm_add_ps(C2, V01);
	C2 = _mm_sub_ps(C2, V01);
	VMVECTOR C5 = _mm_sub_ps(C4, V02);
	C4 = _mm_add_ps(C4, V02);
	VMVECTOR C7 = _mm_add_ps(C6, V03);
	C6 = _mm_sub_ps(C6, V03);

	C0 = _mm_shuffle_ps(C0, C1, _MM_SHUFFLE(3, 1, 2, 0));
	C2 = _mm_shuffle_ps(C2, C3, _MM_SHUFFLE(3, 1, 2, 0));
	C4 = _mm_shuffle_ps(C4, C5, _MM_SHUFFLE(3, 1, 2, 0));
	C6 = _mm_shuffle_ps(C6, C7, _MM_SHUFFLE(3, 1, 2, 0));
	C0 = XM_PERMUTE_PS(C0, _MM_SHUFFLE(3, 1, 2, 0));
	C2 = XM_PERMUTE_PS(C2, _MM_SHUFFLE(3, 1, 2, 0));
	C4 = XM_PERMUTE_PS(C4, _MM_SHUFFLE(3, 1, 2, 0));
	C6 = XM_PERMUTE_PS(C6, _MM_SHUFFLE(3, 1, 2, 0));
	// Get the determinate
	VMVECTOR vTemp = VMVector4Dot(C0, MT.r[0]);
	if (pDeterminant != nullptr)
		*pDeterminant = vTemp;
	vTemp = _mm_div_ps(g_XMOne.v, vTemp);
	VMMATRIX mResult;
	mResult.r[0] = _mm_mul_ps(C0, vTemp);
	mResult.r[1] = _mm_mul_ps(C2, vTemp);
	mResult.r[2] = _mm_mul_ps(C4, vTemp);
	mResult.r[3] = _mm_mul_ps(C6, vTemp);
	return mResult;
}
inline VMMATRIX __vectorcall VMMatrixScalingFromVector
(
	FVMVECTOR Scale
)
{
	VMMATRIX M;
	M.r[0] = _mm_and_ps(Scale, g_XMMaskX.v);
	M.r[1] = _mm_and_ps(Scale, g_XMMaskY.v);
	M.r[2] = _mm_and_ps(Scale, g_XMMaskZ.v);
	M.r[3] = g_XMIdentityR3.v;
	return M;
}
#pragma endregion

