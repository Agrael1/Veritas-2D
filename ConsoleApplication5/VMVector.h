#pragma once
#include "VTypes.h"
#include <assert.h>
#include <math.h>
#include "MathConstants.h"
#include "EngineCommons.h"

// Rounds each component of a vector to the nearest even integer (known as "Bankers Rounding").
inline VMVECTOR __vectorcall VMVectorRound
(
	FVMVECTOR V
)
{
	__m128 sign = _mm_and_ps(V, g_XMNegativeZero.v);
	__m128 sMagic = _mm_or_ps(g_XMNoFraction.v, sign);
	__m128 R1 = _mm_add_ps(V, sMagic);
	R1 = _mm_sub_ps(R1, sMagic);
	__m128 R2 = _mm_and_ps(V, g_XMAbsMask.v);
	__m128 mask = _mm_cmple_ps(R2, g_XMNoFraction.v);
	R2 = _mm_andnot_ps(mask, V);
	R1 = _mm_and_ps(R1, mask);
	VMVECTOR vResult = _mm_xor_ps(R1, R2);
	return vResult;
}

// Vector by matrix multiplication
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


// Replicate the x component of the vector
inline VMVECTOR __vectorcall VMVectorSplatX
(
	FVMVECTOR V
)
{
	return XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));
}


// Replicate the y component of the vector
inline VMVECTOR __vectorcall VMVectorSplatY
(
	FVMVECTOR V
)
{
	return XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
}


// Replicate the z component of the vector
inline VMVECTOR __vectorcall VMVectorSplatZ
(
	FVMVECTOR V
)
{
	return XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
}


// Replicate the w component of the vector
inline VMVECTOR __vectorcall VMVectorSplatW
(
	FVMVECTOR V
)
{
	return XM_PERMUTE_PS(V, _MM_SHUFFLE(3, 3, 3, 3));
}

// Divide Vector by another
inline VMVECTOR __vectorcall VMVectorDivide
(
	FVMVECTOR V1,
	FVMVECTOR V2
)
{
	return _mm_div_ps(V1, V2);
}

// Substract one vector from another
inline VMVECTOR __vectorcall VMVectorSubtract
(
	FVMVECTOR V1,
	FVMVECTOR V2
)
{
	return _mm_sub_ps(V1, V2);
}

// Scales a vector by a scalar
inline VMVECTOR __vectorcall VMVectorScale
(
	FVMVECTOR V,
	float    ScaleFactor
)
{
	VMVECTOR vResult = _mm_set_ps1(ScaleFactor);
	return _mm_mul_ps(vResult, V);
}

// Computes sum of 2 vectors
inline VMVECTOR __vectorcall VMVectorAdd
(
	FVMVECTOR V1,
	FVMVECTOR V2
)
{
	return _mm_add_ps(V1, V2);
}


// Computes the product of the first two vectors added to the third vector.
inline VMVECTOR __vectorcall VMVectorMultiplyAdd
(
	FVMVECTOR V1,
	FVMVECTOR V2,
	FVMVECTOR V3
)
{
	VMVECTOR vResult = _mm_mul_ps(V1, V2);
	return _mm_add_ps(vResult, V3);
}

// Get normalized transformation
inline VMVECTOR __vectorcall VMVector3TransformCoord
(
	FVMVECTOR V,
	FXMMATRIX M
)
{
	VMVECTOR Z = VMVectorSplatZ(V);
	VMVECTOR Y = VMVectorSplatY(V);
	VMVECTOR X = VMVectorSplatX(V);

	VMVECTOR Result = VMVectorMultiplyAdd(Z, M.r[2], M.r[3]);
	Result = VMVectorMultiplyAdd(Y, M.r[1], Result);
	Result = VMVectorMultiplyAdd(X, M.r[0], Result);

	VMVECTOR W = VMVectorSplatW(Result);
	return VMVectorDivide(Result, W);
}

// Loading of Float3 to VMVector
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

// Basic construction of VMVector
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

// Multiply one vec by another
inline VMVECTOR __vectorcall VMVectorMultiply
(
	FVMVECTOR V1,
	FVMVECTOR V2
)
{
	return _mm_mul_ps(V1, V2);
}

// Range angles
inline VMVECTOR __vectorcall VMVectorModAngles
(
	FVMVECTOR Angles
)
{
	// Modulo the range of the given angles such that -XM_PI <= Angles < XM_PI
	VMVECTOR vResult = _mm_mul_ps(Angles, g_XMReciprocalTwoPi.v);
	// Use the inline function due to complexity for rounding
	vResult = VMVectorRound(vResult);
	vResult = _mm_mul_ps(vResult, g_XMTwoPi.v);
	vResult = _mm_sub_ps(Angles, vResult);
	return vResult;
}

// Computes the sine and cosine of each component of an VMVECTOR.
inline void __vectorcall VMVectorSinCos
(
	VMVECTOR* pSin,
	VMVECTOR* pCos,
	FVMVECTOR V
)
{
	assert(pSin != NULL);
	assert(pCos != NULL);

	// 11/10-degree minimax approximation
	// Force the value within the bounds of pi
	VMVECTOR x = VMVectorModAngles(V);

	// Map in [-pi/2,pi/2] with sin(y) = sin(x), cos(y) = sign*cos(x).
	VMVECTOR sign = _mm_and_ps(x, g_XMNegativeZero.v);
	__m128 c = _mm_or_ps(g_XMPi.v, sign);  // pi when x >= 0, -pi when x < 0
	__m128 absx = _mm_andnot_ps(sign, x);  // |x|
	__m128 rflx = _mm_sub_ps(c, x);
	__m128 comp = _mm_cmple_ps(absx, g_XMHalfPi.v);
	__m128 select0 = _mm_and_ps(comp, x);
	__m128 select1 = _mm_andnot_ps(comp, rflx);
	x = _mm_or_ps(select0, select1);
	select0 = _mm_and_ps(comp, g_XMOne.v);
	select1 = _mm_andnot_ps(comp, g_XMNegativeOne.v);
	sign = _mm_or_ps(select0, select1);

	__m128 x2 = _mm_mul_ps(x, x);

	// Compute polynomial approximation of sine
	FVMVECTOR SC1 = g_XMSinCoefficients1.v;
	VMVECTOR vConstants = XM_PERMUTE_PS(SC1, _MM_SHUFFLE(0, 0, 0, 0));
	__m128 Result = _mm_mul_ps(vConstants, x2);

	FVMVECTOR SC0 = g_XMSinCoefficients0.v;
	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(SC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_XMOne.v);
	Result = _mm_mul_ps(Result, x);
	*pSin = Result;

	// Compute polynomial approximation of cosine
	FVMVECTOR CC1 = g_XMCosCoefficients1.v;
	vConstants = XM_PERMUTE_PS(CC1, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_mul_ps(vConstants, x2);

	FVMVECTOR CC0 = g_XMCosCoefficients0.v;
	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(3, 3, 3, 3));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(2, 2, 2, 2));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(1, 1, 1, 1));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);

	vConstants = XM_PERMUTE_PS(CC0, _MM_SHUFFLE(0, 0, 0, 0));
	Result = _mm_add_ps(Result, vConstants);
	Result = _mm_mul_ps(Result, x2);
	Result = _mm_add_ps(Result, g_XMOne.v);
	Result = _mm_mul_ps(Result, sign);
	*pCos = Result;
}

// Permutes the components of two vectors to create a new vector.
inline VMVECTOR __vectorcall VMVectorPermute
(
	FVMVECTOR V1,
	FVMVECTOR V2,
	uint32_t PermuteX,
	uint32_t PermuteY,
	uint32_t PermuteZ,
	uint32_t PermuteW
)
{
	assert(PermuteX <= 7 && PermuteY <= 7 && PermuteZ <= 7 && PermuteW <= 7);
	_Analysis_assume_(PermuteX <= 7 && PermuteY <= 7 && PermuteZ <= 7 && PermuteW <= 7);

	const uint32_t *aPtr[2];
	aPtr[0] = (const uint32_t*)(&V1);
	aPtr[1] = (const uint32_t*)(&V2);

	VMVECTOR Result;
	uint32_t *pWork = (uint32_t*)(&Result);

	const uint32_t i0 = PermuteX & 3;
	const uint32_t vi0 = PermuteX >> 2;
	pWork[0] = aPtr[vi0][i0];

	const uint32_t i1 = PermuteY & 3;
	const uint32_t vi1 = PermuteY >> 2;
	pWork[1] = aPtr[vi1][i1];

	const uint32_t i2 = PermuteZ & 3;
	const uint32_t vi2 = PermuteZ >> 2;
	pWork[2] = aPtr[vi2][i2];

	const uint32_t i3 = PermuteW & 3;
	const uint32_t vi3 = PermuteW >> 2;
	pWork[3] = aPtr[vi3][i3];

	return Result;
}

// Calculate a normal
inline VMVECTOR __vectorcall VMVector3Cross
(
	FVMVECTOR V1,
	FVMVECTOR V2
)
{
	// [ V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x ]
	// y1,z1,x1,w1
	VMVECTOR vTemp1 = XM_PERMUTE_PS(V1, _MM_SHUFFLE(3, 0, 2, 1));
	// z2,x2,y2,w2
	VMVECTOR vTemp2 = XM_PERMUTE_PS(V2, _MM_SHUFFLE(3, 1, 0, 2));
	// Perform the left operation
	VMVECTOR vResult = _mm_mul_ps(vTemp1, vTemp2);
	// z1,x1,y1,w1
	vTemp1 = XM_PERMUTE_PS(vTemp1, _MM_SHUFFLE(3, 0, 2, 1));
	// y2,z2,x2,w2
	vTemp2 = XM_PERMUTE_PS(vTemp2, _MM_SHUFFLE(3, 1, 0, 2));
	// Perform the right operation
	vTemp1 = _mm_mul_ps(vTemp1, vTemp2);
	// Subract the right from left, and return answer
	vResult = _mm_sub_ps(vResult, vTemp1);
	// Set w to zero
	return _mm_and_ps(vResult, g_XMMask3.v);
}

// Dot product between two vectors
inline VMVECTOR __vectorcall VMVector3Dot
(
	FVMVECTOR V1,
	FVMVECTOR V2
)
{
	// Perform the dot product
	VMVECTOR vDot = _mm_mul_ps(V1, V2);
	// x=Dot.vector4_f32[1], y=Dot.vector4_f32[2]
	VMVECTOR vTemp = XM_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
	// Result.vector4_f32[0] = x+y
	vDot = _mm_add_ss(vDot, vTemp);
	// x=Dot.vector4_f32[2]
	vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	// Result.vector4_f32[0] = (x+y)+z
	vDot = _mm_add_ss(vDot, vTemp);
	// Splat x
	return XM_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
}

