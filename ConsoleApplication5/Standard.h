#pragma once


inline void swapptr(void** a, void** b)
{
	void* tmp = *b;
	*b = *a;
	*a = tmp;
}

#define max(a,b) ( a > b ? a : b )
#define min(a,b) ( a < b ? a : b )

// Fill the memory with 4 bytes at a time (note that array must be 4 byte aligned)
void* __cdecl memset32(
	 void*  _Dst,
	 unsigned int    _Val,
	 size_t _Size
);