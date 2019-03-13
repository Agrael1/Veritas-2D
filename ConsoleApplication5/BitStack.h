#pragma once
#include <setjmp.h>

extern const void* Stack;
#if _WIN64
struct Stack
{
	const void* class;

	unsigned long long data[50];
	unsigned int head;
	unsigned char bitctr;

	void(*bPush)(void* self, unsigned char value, unsigned char length);
	short (*bPop)(void* self, unsigned char length);
};

#else
struct Stack
{
	unsigned int data[50];
	unsigned int head;
	unsigned char bitctr;

	void(*bPush)(void* self, unsigned char value, unsigned char length);
	unsigned char(*bPop)(void* self, unsigned char length, jmp_buf exh);
};

#endif