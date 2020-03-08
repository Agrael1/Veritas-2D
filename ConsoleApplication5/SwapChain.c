#include "SwapChain.h"
#include "Class.h"
#include <malloc.h>
#include "Standard.h"
#include <float.h>

void _Clip(selfptr, int*x, int*y)
{
	if (*x > self->nFrameLength) *x = self->nFrameLength;
	if (*y > self->nFrameHeight) *y = self->nFrameHeight;
}
void _PrintFrame(selfptr, int x, int y, CHAR_INFO color)
{
	self->WriteFrame[y*self->nFrameLength + x] = color;
}
bool _DepthTest(selfptr, unsigned x, unsigned y, float zValue)
{
	float* zv = self->ZBuffer + y * self->nFrameLength + x;
	if (zValue < *zv)
	{
		*zv = zValue;
		return true;
	}
	return false;
}

void _ClearFrame(selfptr, CHAR_INFO color)
{
	memset32(self->WriteFrame, *(unsigned int*)&color, (size_t)self->nFrameHeight*self->nFrameLength);
}
void _ClearDepth(selfptr)
{
	union
	{
		unsigned int i;
		float f;
	}imp = {.f = FLT_MAX };
	memset32(self->ZBuffer, imp.i, (size_t)self->nFrameHeight*self->nFrameLength);
}
void _BeginFrame(selfptr, CHAR_INFO color)
{
	_ClearFrame(self, color);
	_ClearDepth(self);
}
void _PresentFrame(selfptr)
{
	swapptr(&self->ReadFrame, &self->WriteFrame);
}

// Old Functions
void _FillRegion(selfptr, int x1, int y1, int x2, int y2, CHAR_INFO color)
{
	_Clip(self, &x1, &y1);
	_Clip(self, &x2, &y2);

	unsigned value = *(unsigned*)&color;

	for (size_t y = y1; y < y2; y++)
		memset32(self->WriteFrame + y*self->nFrameLength + x1, value, x2 - x1);
}
void _DrawRectangle(selfptr, int x1, int y1, int x2, int y2, unsigned short color)
{
	if ((x2 <= x1) && (y2 >= y1))
		_FillRegion(self, x2, y1, x1, y2, (CHAR_INFO) { L' ', color });
	else if ((x2 >= x1) && (y2 <= y1))
		_FillRegion(self, x1, y2, x2, y1, (CHAR_INFO) { L' ', color });
	else if ((x2 <= x1) && (y2 <= y1))
		_FillRegion(self, x2, y2, x1, y1, (CHAR_INFO) { L' ', color });
	else
		_FillRegion(self, x1, y1, x2, y2, (CHAR_INFO) { L' ', color });
}

#define FRAC_BITS 8
void _DrawLine(selfptr, int x1, int y1, int x2, int y2, CHAR_INFO color)
{
	_Clip(self, &x1, &y1);
	_Clip(self, &x2, &y2);

	int dx = x2 - x1;
	int dy = y2 - y1;
	bool isSwapped = false;

	if (!dx && !dy)
		_PrintFrame(self, x1, y1, color);

	if (dy > dx)
	{
		swapptr((void**)&dx, (void**)&dy);
		swapptr((void**)&x1, (void**)&y1);
		swapptr((void**)&x2, (void**)&y2);
		isSwapped = false;
	}
	if (x1 > x2)
	{
		swapptr((void**)&x1, (void**)&x2);
		swapptr((void**)&y1, (void**)&y2);
	}
	
	int y = y1 << FRAC_BITS;
	int k = (dy << FRAC_BITS) / dx;

	if(!isSwapped)
		for (int x = x1; x <= x2; x++)
		{
			_PrintFrame(self, x, y >> FRAC_BITS, color);
			y += k;
		}
	else
		for (int x = x1; x <= x2; x++)
		{
			_PrintFrame(self,  y >> FRAC_BITS, x, color);
			y += k;
		}
}
void _DrawTriangleWireframe(selfptr, int x1, int y1, int x2, int y2, int x3, int y3, CHAR_INFO color)
{
	_DrawLine(self, x1, y1, x2, y2, color);
	_DrawLine(self, x2, y2, x3, y3, color);
	_DrawLine(self, x3, y3, x1, y1, color);
}


VirtualTable{
	.ClearFrame = _ClearFrame,
	.BeginFrame = _BeginFrame,
	.DepthTest = _DepthTest,
	.PresentFrame = _PresentFrame,

	.DrawTriangleWireframe = _DrawTriangleWireframe,
	.DrawLine = _DrawLine,
	.PrintFrame = _PrintFrame,
	.DrawRectangle = _DrawRectangle,
};
Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	Word Width = self->nFrameLength = va_arg(*ap, Word);
	Word Height = self->nFrameHeight = va_arg(*ap, Word);

	self->WriteFrame = _aligned_malloc((size_t)Width * Height * sizeof(CHAR_INFO), alignof(CHAR_INFO));
	self->ReadFrame = _aligned_malloc((size_t)Width * Height * sizeof(CHAR_INFO), alignof(CHAR_INFO));
	self->ZBuffer = _aligned_malloc((size_t)Width * Height * sizeof(float), alignof(float));

	if (self->ZBuffer)
		_ClearDepth(self);
	if (self->WriteFrame)
		_ClearFrame(self, (CHAR_INFO) { L' ', 0 });
	return self;
}
Destructor(selfptr)
{
	_aligned_free(self->WriteFrame);
	_aligned_free(self->ReadFrame);
	_aligned_free(self->ZBuffer);
	return self;
}
ENDCLASSDESC