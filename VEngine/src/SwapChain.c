#define SWAPCHAIN_IMPL
#include "SwapChain.h"
#include <malloc.h>
#include <float.h>

extern inline CHAR_INFO* PresentFrame(selfptr);
extern inline bool DepthTest(selfptr, unsigned x, unsigned y, float zValue);
extern inline void PrintFrame(selfptr, unsigned x, unsigned y, CHAR_INFO color);
extern inline void ClearFrame(selfptr, CHAR_INFO color);
extern inline void ClearDepth(selfptr);
extern inline void BeginFrame(selfptr, CHAR_INFO color);

void Constructor(selfptr, COORD Dimensions)
{
	self->Dimensions = Dimensions;

	_Static_assert(sizeof(CHAR_INFO) == sizeof(float), "Size of CHAR_INFO is not equal to float");
	void* contig_alloc = calloc(3ull * Dimensions.X * Dimensions.Y, sizeof(float));

	self->ZBuffer = (float*)contig_alloc;
	self->ReadFrame = (CHAR_INFO*)contig_alloc + Dimensions.X * Dimensions.Y;
	self->WriteFrame = (CHAR_INFO*)contig_alloc + 2ull * Dimensions.X * Dimensions.Y;
	PresentFrame(self);
}
void Destructor(selfptr)
{
	free(self->ZBuffer); //Single, because block is contiguous
}


//void _Clip(selfptr, int* x, int* y)
//{
//	if (*x > self->nFrameLength) *x = self->nFrameLength;
//	if (*y > self->nFrameHeight) *y = self->nFrameHeight;
//}
//
//
//// Old Functions
//void _FillRegion(selfptr, int x1, int y1, int x2, int y2, CHAR_INFO color)
//{
//	_Clip(self, &x1, &y1);
//	_Clip(self, &x2, &y2);
//
//	unsigned value = *(unsigned*)&color;
//
//	for (size_t y = y1; y < y2; y++)
//		memset32(self->WriteFrame + y * self->nFrameLength + x1, value, x2 - x1);
//}
//void _DrawRectangle(selfptr, int x1, int y1, int x2, int y2, unsigned short color)
//{
//	if ((x2 <= x1) && (y2 >= y1))
//		_FillRegion(self, x2, y1, x1, y2, (CHAR_INFO) { L' ', color });
//	else if ((x2 >= x1) && (y2 <= y1))
//		_FillRegion(self, x1, y2, x2, y1, (CHAR_INFO) { L' ', color });
//	else if ((x2 <= x1) && (y2 <= y1))
//		_FillRegion(self, x2, y2, x1, y1, (CHAR_INFO) { L' ', color });
//	else
//		_FillRegion(self, x1, y1, x2, y2, (CHAR_INFO) { L' ', color });
//}
//
//#define FRAC_BITS 8
//void _DrawLine(selfptr, int x1, int y1, int x2, int y2, CHAR_INFO color)
//{
//	_Clip(self, &x1, &y1);
//	_Clip(self, &x2, &y2);
//
//	int dx = x2 - x1;
//	int dy = y2 - y1;
//	bool isSwapped = false;
//
//	if (!dx && !dy)
//		_PrintFrame(self, x1, y1, color);
//
//	if (dy > dx)
//	{
//		swapptr((void**)&dx, (void**)&dy);
//		swapptr((void**)&x1, (void**)&y1);
//		swapptr((void**)&x2, (void**)&y2);
//		isSwapped = false;
//	}
//	if (x1 > x2)
//	{
//		swapptr((void**)&x1, (void**)&x2);
//		swapptr((void**)&y1, (void**)&y2);
//	}
//
//	int y = y1 << FRAC_BITS;
//	int k = (dy << FRAC_BITS) / dx;
//
//	if (!isSwapped)
//		for (int x = x1; x <= x2; x++)
//		{
//			_PrintFrame(self, x, y >> FRAC_BITS, color);
//			y += k;
//		}
//	else
//		for (int x = x1; x <= x2; x++)
//		{
//			_PrintFrame(self, y >> FRAC_BITS, x, color);
//			y += k;
//		}
//}
//void _DrawTriangleWireframe(selfptr, int x1, int y1, int x2, int y2, int x3, int y3, CHAR_INFO color)
//{
//	_DrawLine(self, x1, y1, x2, y2, color);
//	_DrawLine(self, x2, y2, x3, y3, color);
//	_DrawLine(self, x3, y3, x1, y1, color);
//}