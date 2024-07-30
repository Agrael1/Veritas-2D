// New.h
//
//		GNU GPL License 3.0
//
// Functions for creating and deleting classes.
#pragma once

#ifndef _UINTPTR_T_DEFINED
#define _UINTPTR_T_DEFINED
#ifdef _WIN64
typedef unsigned __int64  uintptr_t;
#else
typedef unsigned int uintptr_t;
#endif
#endif

void construct(void* where, const void* what, ...); //stack creation
void deconstruct(void* self);

void* new (const void* type, ...); // creates instance of class with launching constructor with parameters
void delete(void* item);

size_t sizeOf(const void* _self);
const char* typeOf_t(const void* _type);
const char* typeOf(const void* _self);
uintptr_t typeid(const void* _self);