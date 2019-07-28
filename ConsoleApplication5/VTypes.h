#pragma once

#include <xmmintrin.h>
#include <stdint.h>
typedef __m128 VMVECTOR;
typedef const VMVECTOR FVMVECTOR;

#if (defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM)) && defined(_XM_NO_INTRINSICS_)
typedef struct _VMMATRIX
#else
typedef __declspec(align(16)) struct _VMMATRIX
#endif
{
#ifdef _XM_NO_INTRINSICS_
	union
	{
		XMVECTOR r[4];
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
#else
	VMVECTOR r[4];
#endif
}VMMATRIX;
typedef const VMMATRIX FXMMATRIX;
typedef const VMMATRIX* CXMMATRIX;

//------------------------------------------------------------------------------
// Conversion types for constants
typedef __declspec(align(16)) struct _XMVECTORF32
{
	union
	{
		float f[4];
		VMVECTOR v;
	};
}XMVECTORF32;
typedef __declspec(align(16)) struct _XMVECTORU32
{
	union
	{
		uint32_t u[4];
		VMVECTOR v;
	};
}XMVECTORU32;
typedef __declspec(align(16)) struct _XMVECTORI32
{
	union
	{
		int32_t i[4];
		VMVECTOR v;
	};
}XMVECTORI32;

typedef __declspec(align(16)) struct
{
	float x;
	float y;
	float z;
}VMFLOAT3A;
typedef struct VMFLOAT3
 {
	 float x;
	 float y;
	 float z;
 }VMFLOAT3;
typedef struct
{
	float x;
	float y;
	float z;
	float w;
}VMFLOAT4;

// Standardized Vector
typedef union
{
	VMFLOAT4 c;
	VMVECTOR v;
}SVMVECTOR;

typedef struct
{
	VMVECTOR p[3];
}Triangle;

typedef struct
{
	Triangle *p;
}mesh;
