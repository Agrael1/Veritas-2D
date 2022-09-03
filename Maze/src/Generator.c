#define GEN_IMPL
#include <Generator.h>




typedef enum RoomOri
{

}RoomOri;

static inline uint16_t width(BSPNode* node)
{
	return node->x2 - node->x1;
}
static inline uint16_t height(BSPNode* node)
{
	return node->y2 - node->y1;
}


#define VECTOR_T BSPNode
#include "VectorT.h"
#define VECTOR_T Corridor
#include "VectorT.h"
#define VECTOR_T unsigned
#include "VectorT.h"


__forceinline uint16_t rrand(uint16_t minimum_number, uint16_t max_number)
{
	return rand() % (max_number + 1 - minimum_number) + minimum_number;
}


void SetStartPoint(selfptr, uint8_t startx, uint8_t starty)
{

}

// Gets random start point values
void RandomStartPoint(selfptr)
{

}

void Subdivide(selfptr, uint32_t max_iterations, uint16_t min_width, uint16_t min_height)
{
	Vector(unsigned) nodes_queue;
	Vector(BSPNode) tree;
	Vector_unsigned_ctor(&nodes_queue);
	Vector_BSPNode_ctor(&tree);

	BSPNode* x = push_back_Vector_BSPNode(&tree);
	x->x1 = x->y1 = 0;
	x->x2 = self->width;
	x->y2 = self->height;
	x->left = x->right = NULL;
	*push_back_Vector_unsigned(&nodes_queue) = 0;
	uint32_t iterations = 0;

	while (iterations++ < max_iterations && size_Vector_unsigned(&nodes_queue))
	{
		unsigned at = *front_Vector_unsigned(&nodes_queue);
		BSPNode* xnode = at_Vector_BSPNode(&tree, at);
		size_t cnt = size_Vector_BSPNode(&tree);

		pop_front_Vector_unsigned(&nodes_queue);

		uint16_t nheight = height(xnode);
		uint16_t nwidth = width(xnode);

		Orientation o = ((nwidth > min_width * 2) << 1) | (nheight > min_height * 2);

		if (o == None)continue;
		if (o == Both)o = rrand(1, 2);
		xnode->partition = o;
		xnode->left = (void*)cnt;
		xnode->right = (void*)(cnt + 1);

		uint16_t x1 = xnode->x1, y1 = xnode->y1, x2 = xnode->x2, y2 = xnode->y2;

		BSPNode* u = push_back_Vector_BSPNode(&tree);
		BSPNode* l = push_back_Vector_BSPNode(&tree);
		*push_back_Vector_unsigned(&nodes_queue) = cnt;
		*push_back_Vector_unsigned(&nodes_queue) = cnt + 1;

		u->left = u->right = NULL;
		l->left = l->right = NULL;

		switch (o)
		{
		case Horizontal:
		{
			uint16_t median = rrand(y1 + min_height, y2 - min_height);
			u->x1 = x1;
			u->y1 = y1;
			u->x2 = x2;
			u->y2 = median;

			l->x1 = x1;
			l->y1 = median;
			l->y2 = y2;
			l->x2 = x2;
		}
		break;
		case Vertical:
		{
			uint16_t median = rrand(x1 + min_width, x2 - min_width);
			u->x1 = x1;
			u->y1 = y1;
			u->y2 = y2;
			u->x2 = median;

			l->y1 = y1;
			l->x1 = median;
			l->y2 = y2;
			l->x2 = x2;
		}
		break;
		default:
			break;
		}
	}
	self->graph = tree.data_;
	self->_end = tree.end_;

	// Replace numbers with actual pointers
	for (BSPNode* i = self->graph; i < self->_end; i++)
	{
		if (!i->left)continue;
		i->left = self->graph + (ptrdiff_t)i->left;
		i->right = self->graph + (ptrdiff_t)i->right;
	}

	Vector_unsigned_dtor(&nodes_queue);
}

typedef struct {
	uint16_t min_width, min_height, t_width;
	Vector(Corridor) corridors;
}Desc;

inline void MakeRoom(BSPNode* node, Desc* desc)
{
	uint16_t nwidth = width(node);
	uint16_t nheight = height(node);
	uint16_t xwidth = rrand(desc->min_width, nwidth);
	uint16_t xheight = rrand(desc->min_height, nheight);

	uint16_t px = rrand(0, nwidth - xwidth);
	uint16_t py = rrand(0, nheight - xheight);

	node->x1 += px;
	node->y1 += py;
	node->x2 = node->x1 + xwidth;
	node->y2 = node->y1 + xheight;
}

void ConnectRoomsRec(BSPNode* node, Desc* desc)
{
	if (node->left == NULL && node->right == NULL)
		return MakeRoom(node, desc);
	ConnectRoomsRec(node->left, desc);
	ConnectRoomsRec(node->right, desc);

	Corridor *c = push_back_Vector_Corridor(&desc->corridors);

	//RoomOri orientation = 

	if (node->left->x2 < node->right->x1)
	{// vertical, left is up, right is down

	}
	else
	{// horizontal

	}
}

void MakeRooms(selfptr, uint16_t min_width, uint16_t min_height, uint16_t t_width)
{
	Desc desc = { min_width, min_height, t_width};
	Vector_Corridor_ctor(&desc.corridors);

	ConnectRoomsRec(self->graph, &desc);
	self->corridors = desc.corridors.data_;
	self->c_end = desc.corridors.end_;
}

void Constructor(selfptr, uint8_t width, uint8_t height)
{
	self->height = height;
	self->width = width;
}
void Destructor(selfptr)
{
	if (self->graph)free(self->graph);
	if (self->corridors)free(self->corridors);
}

#include "VectorT.c"