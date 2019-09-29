#pragma once

#include <assert.h>
#include "VMathTypes.h"
#include "MathConstants.h"

inline VMVECTOR __vectorcall VMLoadUInt2
(
	const unsigned* pSource
)
{
	assert(pSource);
	__m128 x = _mm_load_ss((const float*)(pSource));
	__m128 y = _mm_load_ss((const float*)(pSource+1));
	__m128 V = _mm_unpacklo_ps(x, y);
	// For the values that are higher than 0x7FFFFFFF, a fixup is needed
	// Determine which ones need the fix.
	VMVECTOR vMask = _mm_and_ps(V, g_XMNegativeZero.v);
	// Force all values positive
	VMVECTOR vResult = _mm_xor_ps(V, vMask);
	// Convert to floats
	vResult = _mm_cvtepi32_ps(_mm_castps_si128(vResult));
	// Convert 0x80000000 -> 0xFFFFFFFF
	__m128i iMask = _mm_srai_epi32(_mm_castps_si128(vMask), 31);
	// For only the ones that are too big, add the fixup
	vMask = _mm_and_ps(_mm_castsi128_ps(iMask), g_XMFixUnsigned.v);
	vResult = _mm_add_ps(vResult, vMask);
	return vResult;
}