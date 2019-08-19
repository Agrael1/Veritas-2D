#pragma once
#include "MathConstants.h"
#include <math.h>

inline void swapptr(void** a, void** b)
{
	void* tmp = *b;
	*b = *a;
	*a = tmp;
}

inline double clamp(double d, double min, double max) {
	const double t = d < min ? min : d;
	return t > max ? max : t;
}

inline float wrap_angle(float theta)
{
	const float modded = (const float)fmod(theta, XM_2PI);
	return (modded > (float)M_PI) ?
		(modded - XM_2PI) :
		modded;
}

#define max(a,b) ( a > b ? a : b )
#define min(a,b) ( a < b ? a : b )

// Fill the memory with 4 bytes at a time (note that array must be 4 byte aligned)
void* __cdecl memset32(
	 void*  _Dst,
	 unsigned int    _Val,
	 size_t _Size
);