#include "VeritasMath.h"
#include "Color.h"
#include "Frame.h"
#include "Class.h"
#include <math.h>
#include <malloc.h>
#include "Standard.h"
#include <float.h>

void _Clip(selfptr, Word *x, Word *y)
{
	if (*x > self->nFrameLength) *x = self->nFrameLength;
	if (*y > self->nFrameHeight) *y = self->nFrameHeight;
}
void _PrintFrame(selfptr, Word x, Word y, CHAR_INFO color)
{
	self->localFrame[y*self->nFrameLength + x] = color;
}
bool _DepthTest(selfptr, Word x, Word y, float z)
{
	float *zv = self->ZBuffer+y*self->nFrameLength + x;
	if (z < *zv)
	{
		*zv = z;
		return true;
	}
	return false;
}

void _ClearFrame(selfptr, wchar_t c, Word col)
{
	CHAR_INFO r;
	r.Attributes = col;
	r.Char.UnicodeChar = c;
	
	DWord value = *(DWord*)&r;
	memset32(self->localFrame, value, self->nFrameHeight*self->nFrameLength);
}
inline void _ClearDepth(selfptr)
{
	for (int i = 0; i < self->nFrameHeight*self->nFrameLength; i++)
	{
		self->ZBuffer[i] = FLT_MAX;
	}
}
void _BeginFrame(selfptr, wchar_t c, Word col)
{
	_ClearFrame(self, c, col);
	_ClearDepth(self);
}

// Old Functions
void _FillRegion(selfptr, Word x1, Word y1, Word x2, Word y2, wchar_t c, Word col)
{
	_Clip(self, &x1, &y1);
	_Clip(self, &x2, &y2);

	CHAR_INFO r;
	r.Attributes = col;
	r.Char.UnicodeChar = c;

	DWord value = *(DWord*)&r;

	for (int y = y1; y < y2; y++)
		memset32(self->localFrame + y*self->nFrameLength + x1, value, x2 - x1);
}
void _DrawRectangle(selfptr, Word x1, Word y1, Word x2, Word y2, Word color)
{
	if ((x2 <= x1) && (y2 >= y1))
		_FillRegion(self, x2, y1, x1, y2, L' ', color);
	else if ((x2 >= x1) && (y2 <= y1))
		_FillRegion(self, x1, y2, x2, y1, L' ', color);
	else if ((x2 <= x1) && (y2 <= y1))
		_FillRegion(self, x2, y2, x1, y1, L' ', color);
	else
		_FillRegion(self, x1, y1, x2, y2, L' ', color);
}
void _DrawLine(selfptr, Word x1, Word y1, Word x2, Word y2, wchar_t character, Word color)
{
	account(self);
	CHAR_INFO r = { character , color };
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1; y = y1; xe = x2;
		}
		else
		{
			x = x2; y = y2; xe = x1;
		}

		_PrintFrame(this, x, y, r);

		for (i = 0; x<xe; i++)
		{
			x = x + 1;
			if (px<0)
				px = px + 2 * dy1;
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			_PrintFrame(this, x, y, r);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1; y = y1; ye = y2;
		}
		else
		{
			x = x2; y = y2; ye = y1;
		}

		_PrintFrame(this, x, y, r);

		for (i = 0; y<ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			_PrintFrame(this, x, y, r);
		}
	}
}
void _DrawTriangleWireframe(selfptr, Word x1, Word y1, Word x2, Word y2, Word x3, Word y3, wchar_t c, Word col)
{
	_DrawLine(self, x1, y1, x2, y2, c, col);
	_DrawLine(self, x2, y2, x3, y3, c, col);
	_DrawLine(self, x3, y3, x1, y1, c, col);
}


constructMethodTable( 
	.ClearFrame = _ClearFrame,
	.BeginFrame = _BeginFrame,
	.DepthTest = _DepthTest,

	.DrawTriangleWireframe = _DrawTriangleWireframe,
	.DrawLine = _DrawLine,
	.PrintFrame = _PrintFrame,
	.DrawRectangle = _DrawRectangle,
);

Constructor(selfptr, va_list *ap)
{
	account(self);
	assignMethodTable(this);

	this->nFrameLength = va_arg(*ap, Word);
	this->nFrameHeight = va_arg(*ap, Word);

	this->localFrame = malloc(this->nFrameLength * this->nFrameHeight * sizeof(CHAR_INFO));
	this->ZBuffer = malloc(this->nFrameLength*this->nFrameHeight * sizeof(UINT));

	if (this->ZBuffer)
		_ClearDepth(this);
	if (this->localFrame)
		_ClearFrame(this, L' ', 0);

	return this;
}
Destructor(selfptr)
{
	account(self);
	free(this->localFrame);
	return this;
}
ENDCLASSDESC