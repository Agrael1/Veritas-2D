#pragma once
#include "VMVector.h"

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

// Tests whether two 3D vectors are equal.
inline bool __vectorcall VMVector3Equal
(
	FVMVECTOR V1,
	FVMVECTOR V2
)
{
	VMVECTOR vTemp = _mm_cmpeq_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
}

// Checks if vector is infinite
inline bool __vectorcall VMVector3IsInfinite
(
	FVMVECTOR V
)
{
	// Mask off the sign bit
	__m128 vTemp = _mm_and_ps(V, g_XMAbsMask.v);
	// Compare to infinity
	vTemp = _mm_cmpeq_ps(vTemp, g_XMInfinity.v);
	// If x,y or z are infinity, the signs are true.
	return ((_mm_movemask_ps(vTemp) & 7) != 0);
}

// Returns a normalized vector (|V|=1)
inline VMVECTOR __vectorcall VMVector3Normalize
(
	FVMVECTOR V
)
{
	// Perform the dot product on x,y and z only
	VMVECTOR vLengthSq = _mm_mul_ps(V, V);
	VMVECTOR vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	// Prepare for the division
	VMVECTOR vResult = _mm_sqrt_ps(vLengthSq);
	// Create zero with a single instruction
	VMVECTOR vZeroMask = _mm_setzero_ps();
	// Test for a divide by zero (Must be FP to detect -0.0)
	vZeroMask = _mm_cmpneq_ps(vZeroMask, vResult);
	// Failsafe on zero (Or epsilon) length planes
	// If the length is infinity, set the elements to zero
	vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity.v);
	// Divide to perform the normalization
	vResult = _mm_div_ps(V, vResult);
	// Any that are infinity, set to zero
	vResult = _mm_and_ps(vResult, vZeroMask);
	// Select qnan or result based on infinite length
	VMVECTOR vTemp1 = _mm_andnot_ps(vLengthSq, g_XMQNaN.v);
	VMVECTOR vTemp2 = _mm_and_ps(vResult, vLengthSq);
	vResult = _mm_or_ps(vTemp1, vTemp2);
	return vResult;
}