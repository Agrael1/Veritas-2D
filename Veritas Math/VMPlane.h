#pragma once
#include "VMVector.h"
//------------------------------------------------------------------------------
// XMPlaneNormalizeEst uses a reciprocal estimate and
// returns QNaN on zero and infinite vectors.
inline VMVECTOR __vectorcall VMPlaneNormalizeEst
(
	FVMVECTOR P
)
{
	// Perform the dot product
	VMVECTOR vDot = _mm_mul_ps(P, P);
	// x=Dot.y, y=Dot.z
	VMVECTOR vTemp = XM_PERMUTE_PS(vDot, _MM_SHUFFLE(2, 1, 2, 1));
	// Result.x = x+y
	vDot = _mm_add_ss(vDot, vTemp);
	// x=Dot.z
	vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	// Result.x = (x+y)+z
	vDot = _mm_add_ss(vDot, vTemp);
	// Splat x
	vDot = XM_PERMUTE_PS(vDot, _MM_SHUFFLE(0, 0, 0, 0));
	// Get the reciprocal
	vDot = _mm_rsqrt_ps(vDot);
	// Get the reciprocal
	vDot = _mm_mul_ps(vDot, P);
	return vDot;
}

//------------------------------------------------------------------------------
// XMPlaneNormalize
// returns QNaN on zero and infinite vectors.
inline VMVECTOR __vectorcall VMPlaneNormalize
(
	FVMVECTOR P
)
{
	// Perform the dot product on x,y and z only
	VMVECTOR vLengthSq = _mm_mul_ps(P, P);
	VMVECTOR vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(2, 1, 2, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	// Prepare for the division
	VMVECTOR vResult = _mm_sqrt_ps(vLengthSq);
	// Failsafe on zero (Or epsilon) length planes
	// If the length is infinity, set the elements to zero
	vLengthSq = _mm_cmpneq_ps(vLengthSq, g_XMInfinity.v);
	// Reciprocal mul to perform the normalization
	vResult = _mm_div_ps(P, vResult);
	// Any that are infinity, set to zero
	vResult = _mm_and_ps(vResult, vLengthSq);
	return vResult;
}
