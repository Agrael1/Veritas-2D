#pragma once

#ifndef __cplusplus
typedef _Bool bool;
#endif // !__cplusplus

typedef unsigned char Byte;

typedef unsigned short Word;
typedef unsigned int DWord;

#if _WIN64
typedef unsigned long long MaxInt;
#else
typedef unsingned int MaxInt;
#endif

#define true 1
#define false 0
#define nullptr ((void*)0)