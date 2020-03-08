#pragma once

#ifndef __cplusplus
typedef _Bool bool;
#endif // !__cplusplus

typedef unsigned char Byte;
typedef unsigned short Word;

#ifdef _WIN64
typedef unsigned __int64 size_t;
typedef __int64          ptrdiff_t;
typedef __int64          intptr_t;
#else
typedef unsigned int     size_t;
typedef int              ptrdiff_t;
typedef int              intptr_t;
#endif

#define true 1
#define false 0
#define nullptr ((void*)0)