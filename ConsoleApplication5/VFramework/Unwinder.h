#pragma once
#include <stdint.h>
#include <setjmp.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdio.h>
#include "Class.h"

#define ELEMENTS 32

typedef union __unw_element
{
	struct
	{
		uint32_t flag : 1;
		uint32_t value : 31;
	};
	uint32_t entire;
}__unw_element;

typedef struct Unwinder
{
	struct Unwinder* Prev;
	__unw_element pClass[ELEMENTS];
}Unwinder;

typedef struct
{
	Unwinder* block_cur;
	uint8_t nextEl;
	uint8_t blockN;
	const uint8_t offset;
	bool suppressed;
}Context_entry, *pContext_entry;

typedef struct jmp_cache
{
	pContext_entry current_context;
	jmp_buf* env;
	void* __e;
	bool validator;
}jmp_cache;

extern jmp_cache __cache__ex;
extern Unwinder __initial__unw__context;
extern uint32_t _local_el_c;

void __stackvar_insert(const void* cptr, bool allocated);
void __stackvar_extract(const void* cptr);
void __trim_except();
void __release_local_vars();
void __unwinder_suppress();
void __unwinder_release();

#define ProgrammStart \
	Context_entry C = _Make_Context_Entry();\
	__cache__ex.current_context = &C;

#define FunctionPrologue\
	uint32_t __local_c = _local_el_c;\
	_local_el_c = 0
#define FunctionEpilogue\
	__release_local_vars();\
	_local_el_c = __local_c
#define Return FunctionEpilogue; return

inline Context_entry _Make_Context_Entry()
{
	pContext_entry current = __cache__ex.current_context;
	return (Context_entry)
	{ 
		 .block_cur = current->block_cur,
		 .nextEl = current->nextEl,
		 .blockN = 0ui8,
		 .offset = current->nextEl,
		 .suppressed = false
	};
}
