#pragma once
#include <RuntimeClass.h>
#pragma push_macro("c_class")
#undef c_class
#define c_class Generator

typedef struct c_class c_class;

struct c_class
{
	uint8_t width, height;
	uint8_t startx, starty;
};

void Constructor(selfptr, uint8_t width, uint8_t height);
void Destructor(selfptr);


void SetStartPoint(selfptr, uint8_t startx, uint8_t starty);
void RandomStartPoint(selfptr);



void Virtual(Generate)(selfptr);


#ifndef GEN_IMPL
#pragma pop_macro("c_class")
#endif