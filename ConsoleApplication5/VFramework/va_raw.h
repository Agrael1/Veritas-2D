#pragma once
#include <stdarg.h>
#include <memory.h>

#if defined _M_X64
inline void va_raw(void* to, va_list from, size_t bytes)
{
	if (bytes > sizeof(__int64) || (bytes & (bytes - 1)) != 0)
		memcpy(to, *(void**)from, bytes);
	else
		memcpy(to, from, bytes);
}

#elif defined _M_IX86 && !defined _M_HYBRID_X86_ARM64

inline void va_raw(void* to, va_list from, size_t bytes)
{
	memcpy(to, from, bytes);
}

#endif