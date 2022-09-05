#define GEN_IMPL
#include <algorithm>
extern "C"
{
#include <Generator.h>
#define VECTOR_T BSPNode
#include "VectorT.h"
#define VECTOR_T unsigned
#include "VectorT.h"
}

using std::min;
using std::max;


struct BNode : public BSPNode
{
public:
	uint16_t width()const
	{
		return max(x2 - x1, 0);
	}
	uint16_t height()const
	{
		return max(y2 - y1, 0);
	}
	BNode* SearchRecurseV(uint16_t x, uint16_t w, bool lesser)
	{
		if ((int)x < (int)x1 - w || (int)x >(int)x2 - w)return NULL;
		if (corridor) return this;


		switch (partition)
		{
		case None:return this;
		case Vertical:
			if (lesser)
			{
				BNode* a = ((BNode*)right)->SearchRecurseV(x, w, lesser);
				return a ? a : ((BNode*)left)->SearchRecurseV(x, w, lesser);
			}
			[[fallthrough]];
		case Horizontal:
		{
			BNode* a = ((BNode*)left)->SearchRecurseV(x, w, lesser);
			BNode* b = ((BNode*)right)->SearchRecurseV(x, w, lesser);
			if (a && !b)return a;
			if (!a && b)return b;

			return a->y1 > b->y1 ^ lesser ? b : a;
		}
		default:
			return NULL;
		}
	}
	BNode* SearchRecurseH(uint16_t y, uint16_t w, bool lesser)
	{
		if ((int)y < (int)y1 - w || (int)y >(int)y2 - w)return NULL;
		if (corridor) return this;

		switch (partition)
		{
		case None:return this;
		case Horizontal:
			if (lesser)
			{
				BNode* a = ((BNode*)right)->SearchRecurseH(y, w, lesser);
				return a ? a : ((BNode*)left)->SearchRecurseH(y, w, lesser);
			}
			[[fallthrough]];
		case Vertical:
		{
			BNode* a = ((BNode*)left)->SearchRecurseH(y, w, lesser);
			BNode* b = ((BNode*)right)->SearchRecurseH(y, w, lesser);
			if (a && !b)return a;
			if (!a && b)return b;

			return a->x1 > b->x1 ^ lesser ? b : a;
		}
		default:
			return NULL;
		}
	}

	void SplitV(BNode* upper, BNode* lower, uint16_t y)
	{
		upper->x1 = lower->x1 = x1;
		upper->x2 = lower->x2 = x2;
		upper->y1 = y1;
		upper->y2 = lower->y1 = y;
		lower->y2 = y2;
	}
	void SplitH(BNode* left, BNode* right, uint16_t x)
	{
		left->y1 = right->y1 = y1;
		left->y2 = right->y2 = y2;
		left->x1 = x1;
		left->x2 = right->x1 = x;
		right->x2 = x2;
	}
};
class BSPTree
{
public:
	BSPTree() {
		Vector_BSPNode_ctor(&nodes);
	}
	BSPTree(BSPNode* begin, BSPNode* end) {
		nodes.data_ = begin;
		nodes.end_ = end;
		nodes.capacity_ = end - begin;
	}

	~BSPTree() {
		Vector_BSPNode_dtor(&nodes);
	}
public:
	ptrdiff_t emplace()
	{
		auto* x = nodes.data_;
		auto* y = push_back_Vector_BSPNode(&nodes);
		*y = BSPNode{ 0 };
		if (x != nodes.data_)
			Recalculate(x);
		return y - nodes.data_;
	}
	ptrdiff_t size()const
	{
		return nodes.end_ - nodes.data_;
	}
	BNode* operator[](ptrdiff_t index)
	{
		return (BNode*)(nodes.data_ + index);
	}
	ptrdiff_t left(ptrdiff_t a)
	{
		return nodes.data_[a].left - nodes.data_;
	}
	ptrdiff_t right(ptrdiff_t a)
	{
		return nodes.data_[a].right - nodes.data_;
	}
	BSPNode* move_data()
	{
		auto* x = nodes.data_;
		nodes.data_ = NULL;
		return x;
	}
	BSPNode* begin()
	{
		return nodes.data_;
	}
	BSPNode* end()
	{
		return nodes.end_;
	}
private:
	void Recalculate(BSPNode* prev)
	{
		for (BSPNode* i = nodes.data_; i < nodes.end_; i++)
		{
			if (i->left)i->left = i->left - prev + nodes.data_;
			if (i->right)i->right = i->right - prev + nodes.data_;
		}
	}
private:
	Vector(BSPNode) nodes;
};


typedef struct
{
	uint16_t x1, x2;
}Line;
typedef struct {
	uint16_t min_width, min_height, t_width;
	Vector(BSPNode) corridors;
}Desc;

static inline uint16_t width(BSPNode* node)
{
	return node->x2 - node->x1;
}
static inline uint16_t height(BSPNode* node)
{
	return node->y2 - node->y1;
}
static inline Line intersection(Line l1, Line l2)
{
	uint16_t x1 = max(l1.x1, l2.x1);
	uint16_t x2 = min(l1.x2, l2.x2);

	return x2 < x1 ? Line{ 0,0 } : Line{ .x1 = x1, .x2 = x2 };
}




__forceinline uint16_t rrand(uint16_t minimum_number, uint16_t max_number)
{
	return rand() % (max_number + 1 - minimum_number) + minimum_number;
}

void Subdivide(selfptr, uint32_t max_iterations, uint16_t min_width, uint16_t min_height)
{
	Vector(unsigned) nodes_queue;
	BSPTree tree;
	Vector_unsigned_ctor(&nodes_queue);

	auto* x = tree[tree.emplace()];
	x->x2 = self->width;
	x->y2 = self->height;
	*push_back_Vector_unsigned(&nodes_queue) = 0;
	uint32_t iterations = 0;

	while (iterations++ < max_iterations && size_Vector_unsigned(&nodes_queue))
	{
		unsigned at = *front_Vector_unsigned(&nodes_queue);
		auto* x = tree[at];
		size_t cnt = tree.size();

		pop_front_Vector_unsigned(&nodes_queue);
		uint16_t nheight = height(x);
		uint16_t nwidth = width(x);

		Partition o = (Partition)((nwidth > min_width * 2) | ((nheight > min_height * 2) << 1));
		if (o == None)continue;
		if (o == Both)o = (Partition)rrand(1, 2);
		x->partition = o;

		uint16_t x1 = x->x1, y1 = x->y1, x2 = x->x2, y2 = x->y2;

		ptrdiff_t xu = tree.emplace();
		ptrdiff_t xl = tree.emplace();

		*push_back_Vector_unsigned(&nodes_queue) = cnt;
		*push_back_Vector_unsigned(&nodes_queue) = cnt + 1;

		x = tree[at];
		auto* u = tree[xu];
		auto* l = tree[xl];
		x->left = u;
		x->right = l;

		switch (o)
		{
		case Vertical:
			x->SplitV(u, l, rrand(y1 + min_height, y2 - min_height));
			break;
		case Horizontal:
			x->SplitH(u, l, rrand(x1 + min_width, x2 - min_width));
			break;
		default:
			break;
		}
	}
	self->_end = tree.end();
	self->graph = tree.move_data();

	Vector_unsigned_dtor(&nodes_queue);
}
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


//void ConnectRooms(BSPNode* node1, BSPNode* node2, /*out*/ Corridor* c, Orientation o)
//{
//	switch (o)
//	{
//	case Horizontal:
//	{
//		uint16_t ir1 = max(node1->x1, node2->x1);
//		uint16_t ir2 = min(node1->x2, node2->x2);
//
//		uint16_t w = ir2 - ir1;
//		if (w < c->width)return;
//
//		c->x1 = ir1 + rrand(0, w-c->width);
//		c->x2 = c->x1 + c->width;
//		c->y1 = node1->y2;
//		c->y2 = node2->y1;
//	}
//	break;
//	case Vertical:
//	{
//		uint16_t ir1 = max(node1->y1, node2->y1);
//		uint16_t ir2 = min(node1->y2, node2->y2);
//
//		uint16_t h = ir2 - ir1;
//		if (h < c->width)return;
//
//		c->y1 = ir1 + rrand(0, h - c->width);
//		c->y2 = c->y1 + c->width;
//		c->x1 = node1->x2;
//		c->x2 = node2->x1;
//	}
//	break;
//	default:
//		break;
//	}
//}
static inline void RecalculateNode(BSPNode* node)
{
	node->x1 = min(node->left->x1, node->right->x1);
	node->x2 = max(node->left->x2, node->right->x2);

	node->y1 = min(node->left->y1, node->right->y1);
	node->y2 = max(node->left->y2, node->right->y2);
}
void ConnectNodes(BSPTree& tree, BNode* node1, BNode* node2, /*out*/ BSPNode* c, Partition o)
{
	switch (o)
	{
	case Vertical:
	{
		uint16_t ir1 = max(node1->x1, node2->x1);
		uint16_t ir2 = min(node1->x2, node2->x2);

		int16_t w = ir2 - ir1;

		// corner tunnel tbd
		if (w < c->width)
			return;

		c->x1 = ir1 + rrand(0, w - c->width);
		c->x2 = c->x1 + c->width;

		c->y1 = node1->SearchRecurseV(c->x1, c->width, true)->y2;
		c->y2 = node2->SearchRecurseV(c->x1, c->width, false)->y1;
	}
	break;
	case Horizontal:
	{
		uint16_t ir1 = max(node1->y1, node2->y1);
		uint16_t ir2 = min(node1->y2, node2->y2);

		int16_t h = ir2 - ir1;
		if (h < c->width)
			return;

		c->y1 = ir1 + rrand(0, h - c->width);
		c->y2 = c->y1 + c->width;

		c->x1 = node1->SearchRecurseH(c->y1, c->width, true)->x2;
		c->x2 = node2->SearchRecurseH(c->y1, c->width, false)->x1;
	}
	break;
	default:
		break;
	}
}
void ConnectRooms(BSPTree& tree, ptrdiff_t xnode, Desc* desc)
{
	BSPNode* node = tree[xnode];
	if (node->left == NULL && node->right == NULL)
		return MakeRoom(node, desc);

	ptrdiff_t l = tree.left(xnode);
	ptrdiff_t r = tree.right(xnode);
	ConnectRooms(tree, l, desc);
	ConnectRooms(tree, r, desc);

	auto xcorr = tree.emplace();
	auto xdiv = tree.emplace();

	BSPNode* corr = tree[xcorr];
	BSPNode* div = tree[xdiv];
	corr->corridor = true;
	corr->width = desc->t_width;
	node = tree[xnode]; //recalc, because reallocation may have occurred


	ConnectNodes(tree, (BNode*)node->left, (BNode*)node->right, corr, node->partition);

	div->left = node->left;
	div->right = corr;
	div->partition = node->partition;
	node->left = div;

	RecalculateNode(div);
	RecalculateNode(node);
}

void MakeRooms(selfptr, uint16_t min_width, uint16_t min_height, uint16_t t_width)
{
	Desc desc = { min_width, min_height, t_width };
	BSPTree tree{ self->graph, self->_end };

	ConnectRooms(tree, 0, &desc);
	self->_end = tree.end();
	self->graph = tree.move_data();
}

void Constructor(selfptr, uint8_t width, uint8_t height)
{
	self->height = height;
	self->width = width;
}
void Destructor(selfptr)
{
	if (self->graph)free(self->graph);
}

#include "VectorT.c"