// New.h
//
//		GNU GPL License 3.0
//
// Functions for creating and deleting classes.
#pragma once
#include <stdarg.h>
#include <stdint.h>

void construct(void* where, const void* what, ...); //stack creation
void deconstruct(void* self); //use for manual destruction, with RAII is deprecated

void* _new (const void* type, ...); // creates instance of class with launching constructor with parameters
void* _vnew(const void* type, va_list ap);
void _delete(void* item);

#define new _new
#define delete _delete

size_t sizeOf(const void* _self);
const char* typeOf_t(const void* _type);
const char* typeOf(const void* _self);
uintptr_t typeid(const void* _self);