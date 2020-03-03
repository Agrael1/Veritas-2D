// New.h
//
//		GNU GPL License 3.0
//
// Functions for creating and deleting classes.
#pragma once
void construct(void* where, const void* what, ...); //stack creation
void deconstruct(void* self);

void* new (const void* type, ...); // creates instance of class with launching constructor with parameters
void delete(void* item);

unsigned int sizeOf(const void* _self);
const char* typeOf_t(const void* _type);
const char* typeOf(const void* _self);