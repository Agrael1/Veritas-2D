#pragma once
inline void swapptr(void** a, void** b)
{
	void* tmp = *b;
	*b = *a;
	*a = tmp;
}