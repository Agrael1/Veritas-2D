#pragma once
inline void swapptr(void** a, void** b)
{
	void* tmp = *b;
	*b = *a;
	*a = tmp;
}

// Fill the memory with 4 bytes at a time (note that array must be 4 byte aligned)
void* __cdecl memset32(
	_Out_writes_bytes_all_(_Size) void*  _Dst,
	_In_                          unsigned int    _Val,
	_In_                          size_t _Size
);