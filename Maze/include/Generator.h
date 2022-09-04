#pragma once
#include <RuntimeClass.h>
#include <stdbool.h>
#pragma push_macro("c_class")
#undef c_class
#define c_class Generator

typedef struct c_class c_class;


typedef enum Partition
{
	None,
	Horizontal, //< [l  |  r]
	Vertical, //< ^{u  |  l}v
	Both
}Partition;
typedef struct BSPNode
{
	uint16_t x1, y1; //top left
	uint16_t x2, y2; //bottom right
	union
	{
		Partition partition : 16;
		uint16_t width;
	};
	bool corridor;
	struct BSPNode* left, * right;
}BSPNode, * PBSPNode;
typedef struct
{
	uint16_t x1, y1; //top left
	uint16_t x2, y2; //bottom right
	uint16_t width;
}Corridor;

struct c_class
{
	uint16_t width, height;
	BSPNode* graph;
	BSPNode* _end;
};

void Constructor(selfptr, uint8_t width, uint8_t height);
void Destructor(selfptr);


void SetStartPoint(selfptr, uint8_t startx, uint8_t starty);
void RandomStartPoint(selfptr);
void Subdivide(selfptr, uint32_t max_iterations, uint16_t min_width, uint16_t min_height);
void MakeRooms(selfptr, uint16_t min_width, uint16_t min_height, uint16_t t_width);


void Virtual(Generate)(selfptr);


#ifndef GEN_IMPL
#pragma pop_macro("c_class")
#endif