/*
	Library part for store and load functions
*/

#pragma once
#include <assert.h>
#include "VEngineTypes.h"
#include "VMathTypes.h"
#include "MathConstants.h"

// Load Float3A into VMVECTOR
inline VMVECTOR __vectorcall VMLoadFloat3A
(
	const VMFLOAT3A* pSource
)
{
	__m128 V = _mm_load_ps(&pSource->x);
	return _mm_and_ps(V, g_XMMask3.v);
}

// Loading of unaligned Float3 to VMVector
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

// Load Float4A into VMVECTOR
inline VMVECTOR __vectorcall VMLoadFloat4A
(
	const VMFLOAT4A* pSource
)
{
	assert(pSource);
	assert(((uintptr_t)pSource & 0xF) == 0);
	return _mm_load_ps(&pSource->x);
}

// Store float3A from VMVector
inline void __vectorcall VMStoreFloat3A
(
	VMFLOAT3A*   pDestination,
	FVMVECTOR     V
)
{
	assert(pDestination);
	assert(((uintptr_t)pDestination & 0xF) == 0);

	VMVECTOR T = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_storel_epi64((__m128i*)(pDestination), _mm_castps_si128(V));
	_mm_store_ss(&pDestination->z, T);
}

// Stores Vector into float3
inline void __vectorcall VMStoreFloat3
(
	VMFLOAT3* pDestination,
	FVMVECTOR V
)
{
	assert(pDestination);
	VMVECTOR T1 = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
	VMVECTOR T2 = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
	_mm_store_ss(&pDestination->x, V);
	_mm_store_ss(&pDestination->y, T1);
	_mm_store_ss(&pDestination->z, T2);
}

// Stores Vector into float4 checking for proper alignment
inline void __vectorcall VMStoreFloat4A
(
	VMFLOAT4A*   pDestination,
	FVMVECTOR     V
)
{
	assert(pDestination);
	assert(((uintptr_t)pDestination & 0xF) == 0);
	_mm_store_ps(&pDestination->x, V);
}