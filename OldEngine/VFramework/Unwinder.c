#include "Unwinder.h"
#include <stdlib.h>

Unwinder __initial__unw__context = { .Prev = NULL, {0} };
Context_entry __programm_start = { .block_cur = &__initial__unw__context, .blockN = 0, .suppressed = false, .nextEl = 0 };

jmp_cache __cache__ex = { .current_context = &__programm_start, .env = NULL, .__e = NULL, .validator = false };
uint32_t _local_el_c = 0;

void __stackvar_insert(const void* cptr, bool allocated)
{
	pContext_entry current = __cache__ex.current_context;
	if (current->suppressed) return;
	if (current->nextEl == ELEMENTS)
	{
		current->nextEl = 0;
		Unwinder* next = calloc(1u, sizeof(Unwinder));
		next->Prev = current->block_cur;
		current->block_cur = next;
		current->blockN++;
	}
	__unw_element entry = { .flag = allocated?1:0, .value = (unsigned char*)current - (unsigned char*)cptr };
	current->block_cur->pClass[current->nextEl++] = entry;

	_local_el_c++;
	//TODO: array proxy
}
void __stackvar_extract(const void* cptr)
{
	pContext_entry current = __cache__ex.current_context;
	if (current->suppressed) return;
	Unwinder* pUnwind = __cache__ex.current_context->block_cur;
	int32_t diff = (unsigned char*)current - (unsigned char*)cptr;

	for (uint8_t i = current->nextEl - 1, block = current->blockN; ; i--)
	{
		if (!pUnwind->pClass[i].entire) continue;
		if (pUnwind->pClass[i].value == diff)
		{
			pUnwind->pClass[i].entire = 0ui32;
			break;
		}
		if (i == 0)
		{
			if (!block) return;
			block--;
			if (!(pUnwind = pUnwind->Prev)) // scan if we are not at the end of the line
				return;
			i = 32; // go on from previous block
		}
	}
}
void __trim_except()
{
	pContext_entry current = __cache__ex.current_context;	// all context elements are (nextEL)+ELEMENTS*BlockN-offset;
	Unwinder* pUnwind = current->block_cur;
	bool breakCond = current->blockN == 0;
	__unw_element entry = { .entire = 0 };

	if ((current->nextEl == current->offset) && !breakCond) return;

	for (int i = current->nextEl - 1;(!breakCond)||i!=current->offset-1; i--)
	{
		entry = pUnwind->pClass[i];
		if (entry.entire ^ 1)
		{
			struct Class** pClass = (struct Class**)((unsigned char*)current - entry.value);
			(*pClass)->dtor(pClass);
			pUnwind->pClass[i].entire = 0ui32;	//not so important
		}
		else
		{
			struct Class** pClass = *(struct Class**)((unsigned char*)current - entry.value);
			if ((*pClass)->dtor) (*pClass)->dtor(pClass);
			free(pClass);
		}

		if (i == 0&&!breakCond)
		{
			if (current->blockN == 1ui8) breakCond = true;
			current->blockN--;
			if (pUnwind != &__initial__unw__context)
			{
				pUnwind = pUnwind->Prev;
				free(current->block_cur);
				current->block_cur = pUnwind;
			}
			i = 32; // go on from previous block
		}
	}
}
void __release_local_vars()
{
	pContext_entry current = __cache__ex.current_context;
	Unwinder* pUnwind = current->block_cur;
	int i = current->nextEl - 1;
	__unw_element entry = { .entire = 0 };

	for (; _local_el_c--; i--)
	{
		entry = pUnwind->pClass[i];
		if (!(entry.entire & 1))
		{
			struct Class** pClass = (struct Class**)((unsigned char*)current - entry.value);
			if ((*pClass)->dtor) (*pClass)->dtor(pClass);
			pUnwind->pClass[i].entire = 0ui32;
		}
		else
		{
			struct Class** pClass = *(struct Class**)((unsigned char*)current - entry.value);
			if ((*pClass)->dtor) (*pClass)->dtor(pClass);
			free(pClass);
		}

		if (i == 0)
		{
			if (!current->blockN) return;
			current->blockN--;
			if (pUnwind != &__initial__unw__context)
			{
				pUnwind = pUnwind->Prev;
				free(current->block_cur);
				current->block_cur = pUnwind;
			}
			i = 32; // go on from previous block
		}
	}
	current->nextEl = i;
}
void __unwinder_suppress()
{
	__cache__ex.current_context->suppressed = true;
}
void __unwinder_release()
{
	__cache__ex.current_context->suppressed = false;
}

extern inline Context_entry _Make_Context_Entry();