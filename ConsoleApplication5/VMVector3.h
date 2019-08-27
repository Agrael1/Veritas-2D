#pragma once
#include "VMVector.h"

// Predecl
inline VMMATRIX __vectorcall VMMatrixMultiply(FXMMATRIX M1, CXMMATRIX M2);

// Load Float3A into VMVECTOR
inline VMVECTOR __vectorcall VMLoadFloat3A(const VMFLOAT3A* pSource)
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



// Projects vector from world to screen space
inline VMVECTOR __vectorcall VMVector3Project
(
	FVMVECTOR V,
	float    ViewportX,
	float    ViewportY,
	float    ViewportWidth,
	float    ViewportHeight,
	float    ViewportMinZ,
	float    ViewportMaxZ,
	FXMMATRIX Projection,
	CXMMATRIX View,
	CXMMATRIX World
)
{
	const float HalfViewportWidth = ViewportWidth * 0.5f;
	const float HalfViewportHeight = ViewportHeight * 0.5f;

	VMVECTOR Scale = VMVectorSet(HalfViewportWidth, -HalfViewportHeight, ViewportMaxZ - ViewportMinZ, 0.0f);
	VMVECTOR Offset = VMVectorSet(ViewportX + HalfViewportWidth, ViewportY + HalfViewportHeight, ViewportMinZ, 0.0f);

	VMMATRIX Transform = VMMatrixMultiply(*World, View);
	Transform = VMMatrixMultiply(Transform, &Projection);

	VMVECTOR Result = VMVector3TransformCoord(V, Transform);

	Result = VMVectorMultiplyAdd(Result, Scale, Offset);

	return Result;
}

// Transforms normal, ignoring 4th component of the matrix
inline VMVECTOR __vectorcall VMVector3TransformNormal
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
	return vResult;
}

// Compares 2 vectors <=
inline bool __vectorcall VMVector3LessOrEqual
(
	FVMVECTOR V1,
	FVMVECTOR V2
)
{
	VMVECTOR vTemp = _mm_cmple_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & 7) == 7) != 0);
}

// Masked comparison less
inline bool __vectorcall VMVector3GreaterMasked
(
	FVMVECTOR V1,
	FVMVECTOR V2,
	uint8_t mask
)
{
	VMVECTOR vTemp = _mm_cmpgt_ps(V1, V2);
	return (((_mm_movemask_ps(vTemp) & mask) == mask));
}

// Transforms coordinates in stream fashion
inline VMFLOAT4* __vectorcall VMVector3TransformStream
(
	VMFLOAT4*       pOutputStream,
	size_t          OutputStride,
	const VMFLOAT3* pInputStream,
	size_t          InputStride,
	size_t          VectorCount,
	FXMMATRIX       M
)
{
	assert(pOutputStream != nullptr);
	assert(pInputStream != nullptr);

	assert(InputStride >= sizeof(VMFLOAT3));
	_Analysis_assume_(InputStride >= sizeof(VMFLOAT3));

	assert(OutputStride >= sizeof(VMFLOAT4));
	_Analysis_assume_(OutputStride >= sizeof(VMFLOAT4));


	const uint8_t* pInputVector = (const uint8_t*)pInputStream;
	uint8_t* pOutputVector = (uint8_t*)pOutputStream;

	FVMVECTOR row0 = M.r[0];
	FVMVECTOR row1 = M.r[1];
	FVMVECTOR row2 = M.r[2];
	FVMVECTOR row3 = M.r[3];

	size_t i = 0;
	size_t four = VectorCount >> 2;
	if (four > 0)
	{
		if (InputStride == sizeof(VMFLOAT3))
		{
			if (!((uintptr_t)pOutputStream & 0xF) && !(OutputStride & 0xF))
			{
				// Packed input, aligned output
				for (size_t j = 0; j < four; ++j)
				{
					__m128 V1 = _mm_loadu_ps((const float*)(pInputVector));
					__m128 L2 = _mm_loadu_ps((const float*)(pInputVector + 16));
					__m128 L3 = _mm_loadu_ps((const float*)(pInputVector + 32));
					pInputVector += sizeof(VMFLOAT3) * 4;

					// Unpack the 4 vectors (.w components are junk)
					XM3UNPACK3INTO4(V1, L2, L3);

					// Result 1
					VMVECTOR Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
					VMVECTOR Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
					VMVECTOR X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

					VMVECTOR vTemp = _mm_mul_ps(Z, row2);
					VMVECTOR vTemp2 = _mm_mul_ps(Y, row1);
					VMVECTOR vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					XM_STREAM_PS((float*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					// Result 2
					Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
					Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
					X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					XM_STREAM_PS((float*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					// Result 3
					Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
					Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
					X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					XM_STREAM_PS((float*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					// Result 4
					Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
					Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
					X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					XM_STREAM_PS((float*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					i += 4;
				}
			}
			else
			{
				// Packed input, unaligned output
				for (size_t j = 0; j < four; ++j)
				{
					__m128 V1 = _mm_loadu_ps((const float*)(pInputVector));
					__m128 L2 = _mm_loadu_ps((const float*)(pInputVector + 16));
					__m128 L3 = _mm_loadu_ps((const float*)(pInputVector + 32));
					pInputVector += sizeof(VMFLOAT3) * 4;

					// Unpack the 4 vectors (.w components are junk)
					XM3UNPACK3INTO4(V1, L2, L3);

					// Result 1
					VMVECTOR Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
					VMVECTOR Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
					VMVECTOR X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

					VMVECTOR vTemp = _mm_mul_ps(Z, row2);
					VMVECTOR vTemp2 = _mm_mul_ps(Y, row1);
					VMVECTOR vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					_mm_storeu_ps((float*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					// Result 2
					Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
					Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
					X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					_mm_storeu_ps((float*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					// Result 3
					Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
					Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
					X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					_mm_storeu_ps((float*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					// Result 4
					Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
					Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
					X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, row3);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);
					_mm_storeu_ps((float*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					i += 4;
				}
			}
		}
	}

	if (!((uintptr_t)pOutputStream & 0xF) && !(OutputStride & 0xF))
	{
		// Aligned output
		for (; i < VectorCount; ++i)
		{
#pragma prefast( suppress : 26019, "PREfast noise: Esp:1307" )
			VMVECTOR V = VMLoadFloat3((const VMFLOAT3*)(pInputVector));
			pInputVector += InputStride;

			VMVECTOR Z = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			VMVECTOR Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			VMVECTOR X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

			VMVECTOR vTemp = _mm_mul_ps(Z, row2);
			VMVECTOR vTemp2 = _mm_mul_ps(Y, row1);
			VMVECTOR vTemp3 = _mm_mul_ps(X, row0);
			vTemp = _mm_add_ps(vTemp, row3);
			vTemp = _mm_add_ps(vTemp, vTemp2);
			vTemp = _mm_add_ps(vTemp, vTemp3);

			XM_STREAM_PS((float*)(pOutputVector), vTemp);
			pOutputVector += OutputStride;
		}
	}
	else
	{
		// Unaligned output
		for (; i < VectorCount; ++i)
		{
#pragma prefast( suppress : 26019, "PREfast noise: Esp:1307" )
			VMVECTOR V = VMLoadFloat3((const VMFLOAT3*)(pInputVector));
			pInputVector += InputStride;

			VMVECTOR Z = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
			VMVECTOR Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
			VMVECTOR X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

			VMVECTOR vTemp = _mm_mul_ps(Z, row2);
			VMVECTOR vTemp2 = _mm_mul_ps(Y, row1);
			VMVECTOR vTemp3 = _mm_mul_ps(X, row0);
			vTemp = _mm_add_ps(vTemp, row3);
			vTemp = _mm_add_ps(vTemp, vTemp2);
			vTemp = _mm_add_ps(vTemp, vTemp3);

			_mm_storeu_ps((float*)(pOutputVector), vTemp);
			pOutputVector += OutputStride;
		}
	}

	return pOutputStream;
}

// Transforms array of normals 
inline VMFLOAT3* __vectorcall VMVector3TransformNormalStream
(
	VMFLOAT3*       pOutputStream,
	size_t          OutputStride,
	const VMFLOAT3* pInputStream,
	size_t          InputStride,
	size_t          VectorCount,
	FXMMATRIX       M
)
{
	assert(pOutputStream != nullptr);
	assert(pInputStream != nullptr);

	assert(InputStride >= sizeof(VMFLOAT3));
	_Analysis_assume_(InputStride >= sizeof(VMFLOAT3));

	assert(OutputStride >= sizeof(VMFLOAT3));
	_Analysis_assume_(OutputStride >= sizeof(VMFLOAT3));


	const uint8_t* pInputVector = (const uint8_t*)pInputStream;
	uint8_t* pOutputVector = (uint8_t*)pOutputStream;

	FVMVECTOR row0 = M.r[0];
	FVMVECTOR row1 = M.r[1];
	FVMVECTOR row2 = M.r[2];

	size_t i = 0;
	size_t four = VectorCount >> 2;
	if (four > 0)
	{
		if (InputStride == sizeof(VMFLOAT3))
		{
			if (OutputStride == sizeof(VMFLOAT3))
			{
				if (!((uintptr_t)pOutputStream & 0xF))
				{
					// Packed input, aligned & packed output
					for (size_t j = 0; j < four; ++j)
					{
						__m128 V1 = _mm_loadu_ps((const float*)(pInputVector));
						__m128 L2 = _mm_loadu_ps((const float*)(pInputVector + 16));
						__m128 L3 = _mm_loadu_ps((const float*)(pInputVector + 32));
						pInputVector += sizeof(VMFLOAT3) * 4;

						// Unpack the 4 vectors (.w components are junk)
						XM3UNPACK3INTO4(V1, L2, L3);

						// Result 1
						VMVECTOR Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
						VMVECTOR Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
						VMVECTOR X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

						VMVECTOR vTemp = _mm_mul_ps(Z, row2);
						VMVECTOR vTemp2 = _mm_mul_ps(Y, row1);
						VMVECTOR vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V1 = _mm_add_ps(vTemp, vTemp3);

						// Result 2
						Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
						Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
						X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V2 = _mm_add_ps(vTemp, vTemp3);

						// Result 3
						Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
						Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
						X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V3 = _mm_add_ps(vTemp, vTemp3);

						// Result 4
						Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
						Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
						X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V4 = _mm_add_ps(vTemp, vTemp3);

						// Pack and store the vectors
						XM3PACK4INTO3(vTemp);
						XM_STREAM_PS((float*)(pOutputVector), V1);
						XM_STREAM_PS((float*)(pOutputVector + 16), vTemp);
						XM_STREAM_PS((float*)(pOutputVector + 32), V3);
						pOutputVector += sizeof(VMFLOAT3) * 4;
						i += 4;
					}
				}
				else
				{
					// Packed input, unaligned & packed output
					for (size_t j = 0; j < four; ++j)
					{
						__m128 V1 = _mm_loadu_ps((const float*)(pInputVector));
						__m128 L2 = _mm_loadu_ps((const float*)(pInputVector + 16));
						__m128 L3 = _mm_loadu_ps((const float*)(pInputVector + 32));
						pInputVector += sizeof(VMFLOAT3) * 4;

						// Unpack the 4 vectors (.w components are junk)
						XM3UNPACK3INTO4(V1, L2, L3);

						// Result 1
						VMVECTOR Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
						VMVECTOR Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
						VMVECTOR X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

						VMVECTOR vTemp = _mm_mul_ps(Z, row2);
						VMVECTOR vTemp2 = _mm_mul_ps(Y, row1);
						VMVECTOR vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V1 = _mm_add_ps(vTemp, vTemp3);

						// Result 2
						Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
						Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
						X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V2 = _mm_add_ps(vTemp, vTemp3);

						// Result 3
						Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
						Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
						X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V3 = _mm_add_ps(vTemp, vTemp3);

						// Result 4
						Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
						Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
						X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

						vTemp = _mm_mul_ps(Z, row2);
						vTemp2 = _mm_mul_ps(Y, row1);
						vTemp3 = _mm_mul_ps(X, row0);
						vTemp = _mm_add_ps(vTemp, vTemp2);
						V4 = _mm_add_ps(vTemp, vTemp3);

						// Pack and store the vectors
						XM3PACK4INTO3(vTemp);
						_mm_storeu_ps((float*)(pOutputVector), V1);
						_mm_storeu_ps((float*)(pOutputVector + 16), vTemp);
						_mm_storeu_ps((float*)(pOutputVector + 32), V3);
						pOutputVector += sizeof(VMFLOAT3) * 4;
						i += 4;
					}
				}
			}
			else
			{
				// Packed input, unpacked output
				for (size_t j = 0; j < four; ++j)
				{
					__m128 V1 = _mm_loadu_ps((const float*)(pInputVector));
					__m128 L2 = _mm_loadu_ps((const float*)(pInputVector + 16));
					__m128 L3 = _mm_loadu_ps((const float*)(pInputVector + 32));
					pInputVector += sizeof(VMFLOAT3) * 4;

					// Unpack the 4 vectors (.w components are junk)
					XM3UNPACK3INTO4(V1, L2, L3);

					// Result 1
					VMVECTOR Z = XM_PERMUTE_PS(V1, _MM_SHUFFLE(2, 2, 2, 2));
					VMVECTOR Y = XM_PERMUTE_PS(V1, _MM_SHUFFLE(1, 1, 1, 1));
					VMVECTOR X = XM_PERMUTE_PS(V1, _MM_SHUFFLE(0, 0, 0, 0));

					VMVECTOR vTemp = _mm_mul_ps(Z, row2);
					VMVECTOR vTemp2 = _mm_mul_ps(Y, row1);
					VMVECTOR vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);

#pragma prefast( suppress : 26015, "PREfast noise: Esp:1307" )
					VMStoreFloat3((VMFLOAT3*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					// Result 2
					Z = XM_PERMUTE_PS(V2, _MM_SHUFFLE(2, 2, 2, 2));
					Y = XM_PERMUTE_PS(V2, _MM_SHUFFLE(1, 1, 1, 1));
					X = XM_PERMUTE_PS(V2, _MM_SHUFFLE(0, 0, 0, 0));

					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);

#pragma prefast( suppress : 26015, "PREfast noise: Esp:1307" )
					VMStoreFloat3((VMFLOAT3*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					// Result 3
					Z = XM_PERMUTE_PS(V3, _MM_SHUFFLE(2, 2, 2, 2));
					Y = XM_PERMUTE_PS(V3, _MM_SHUFFLE(1, 1, 1, 1));
					X = XM_PERMUTE_PS(V3, _MM_SHUFFLE(0, 0, 0, 0));

					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);

#pragma prefast( suppress : 26015, "PREfast noise: Esp:1307" )
					VMStoreFloat3((VMFLOAT3*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					// Result 4
					Z = XM_PERMUTE_PS(V4, _MM_SHUFFLE(2, 2, 2, 2));
					Y = XM_PERMUTE_PS(V4, _MM_SHUFFLE(1, 1, 1, 1));
					X = XM_PERMUTE_PS(V4, _MM_SHUFFLE(0, 0, 0, 0));

					vTemp = _mm_mul_ps(Z, row2);
					vTemp2 = _mm_mul_ps(Y, row1);
					vTemp3 = _mm_mul_ps(X, row0);
					vTemp = _mm_add_ps(vTemp, vTemp2);
					vTemp = _mm_add_ps(vTemp, vTemp3);

#pragma prefast( suppress : 26015, "PREfast noise: Esp:1307" )
					VMStoreFloat3((VMFLOAT3*)(pOutputVector), vTemp);
					pOutputVector += OutputStride;

					i += 4;
				}
			}
		}
	}

	for (; i < VectorCount; i++)
	{
#pragma prefast( suppress : 26019, "PREfast noise: Esp:1307" )
		VMVECTOR V = VMLoadFloat3((const VMFLOAT3*)(pInputVector));
		pInputVector += InputStride;

		VMVECTOR Z = XM_PERMUTE_PS(V, _MM_SHUFFLE(2, 2, 2, 2));
		VMVECTOR Y = XM_PERMUTE_PS(V, _MM_SHUFFLE(1, 1, 1, 1));
		VMVECTOR X = XM_PERMUTE_PS(V, _MM_SHUFFLE(0, 0, 0, 0));

		VMVECTOR vTemp = _mm_mul_ps(Z, row2);
		VMVECTOR vTemp2 = _mm_mul_ps(Y, row1);
		VMVECTOR vTemp3 = _mm_mul_ps(X, row0);
		vTemp = _mm_add_ps(vTemp, vTemp2);
		vTemp = _mm_add_ps(vTemp, vTemp3);

#pragma prefast( suppress : 26015, "PREfast noise: Esp:1307" )
		VMStoreFloat3((VMFLOAT3*)(pOutputVector), vTemp);
		pOutputVector += OutputStride;
	}

	return pOutputStream;
}

inline VMVECTOR __vectorcall VMVector3LengthEst
(
	FVMVECTOR V
)
{
	// Perform the dot product on x,y and z
	VMVECTOR vLengthSq = _mm_mul_ps(V, V);
	// vTemp has z and y
	VMVECTOR vTemp = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(1, 2, 1, 2));
	// x+z, y
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	// y,y,y,y
	vTemp = XM_PERMUTE_PS(vTemp, _MM_SHUFFLE(1, 1, 1, 1));
	// x+z+y,??,??,??
	vLengthSq = _mm_add_ss(vLengthSq, vTemp);
	// Splat the length squared
	vLengthSq = XM_PERMUTE_PS(vLengthSq, _MM_SHUFFLE(0, 0, 0, 0));
	// Get the length
	vLengthSq = _mm_sqrt_ps(vLengthSq);
	return vLengthSq;
}
