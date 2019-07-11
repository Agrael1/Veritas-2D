#include "VeritasMath.h"
#include "Frame.h"
#include "Class.h"
#include <math.h>
#include <malloc.h>
#include "Standard.h"


void _Clip(selfptr, Word *x, Word *y)
{
	if (*x > self->nFrameLength) *x = self->nFrameLength;
	if (*y > self->nFrameHeight) *y = self->nFrameHeight;
}
// Deprecated
void _PrintFrame(selfptr, Word x, Word y, wchar_t character, Word color)
{
	account(self);
	if (x < this->nFrameLength&&y < this->nFrameHeight)
	{
		this->localFrame[y*this->nFrameLength + x].Char.UnicodeChar = character;
		this->localFrame[y*this->nFrameLength + x].Attributes = color;
	}
}
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

		_PrintFrame(this, x, y, character, color);

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
			_PrintFrame(this, x, y, character, color);
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

		_PrintFrame(this, x, y, character, color);

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
			_PrintFrame(this, x, y, character, color);
		}
	}
}
void _DrawTriangleWireframe(selfptr, Word x1, Word y1, Word x2, Word y2, Word x3, Word y3, wchar_t c, Word col)
{
	_DrawLine(self, x1, y1, x2, y2, c, col);
	_DrawLine(self, x2, y2, x3, y3, c, col);
	_DrawLine(self, x3, y3, x1, y1, c, col);
}

// inlines for fast render (scanlines, 0.5 is for pixel centers)
inline void DrawFlatTopTriangle(selfptr, const VMVECTOR* v0, const VMVECTOR* v1, const VMVECTOR* v2, wchar_t c, Word col)
{
	// calculate slopes in screen space
	VMVECTOR res = VMVectorSubtract(*v2, *v0);
	const float m0 = res.m128_f32[0] / res.m128_f32[1];
	res = VMVectorSubtract(*v2, *v1);
	const float m1 = res.m128_f32[0] / res.m128_f32[1];

	// calculate start and end scanlines
	const int yStart = (int)ceilf(v0->m128_f32[1] - 0.5f);
	const int yEnd = (int)ceilf(v2->m128_f32[1] - 0.5f);

	for (int y = yStart; y < yEnd; y++)
	{
		// calculate start and end points(x coord)
		const float px0 = m0*((float)y + 0.5f - v0->m128_f32[1]) + v0->m128_f32[0];
		const float px1 = m1*((float)y + 0.5f - v1->m128_f32[1]) + v1->m128_f32[0];

		// start and end pixels
		const int xStart = (int)ceil(px0 - 0.5f);
		const int xEnd = (int)ceil(px1 - 0.5f);

		CHAR_INFO r;
		r.Attributes = col;
		r.Char.UnicodeChar = c;
		DWord value = *(DWord*)&r;

		if (xEnd - xStart)
			memset32(self->localFrame + y*self->nFrameLength + xStart, value, xEnd - xStart);
	}
}
inline void DrawFlatBottomTriangle(selfptr, const VMVECTOR* v0, const VMVECTOR* v1, const VMVECTOR* v2, wchar_t c, Word col)
{
	// calculate slopes in screen space
	VMVECTOR res = VMVectorSubtract(*v1, *v0);
	const float m0 = res.m128_f32[0] / res.m128_f32[1];
	res = VMVectorSubtract(*v2, *v0);
	const float m1 = res.m128_f32[0] / res.m128_f32[1];

	// calculate start and end scanlines
	const int yStart = (int)ceilf(v0->m128_f32[1] - 0.5f);
	const int yEnd = (int)ceilf(v2->m128_f32[1] - 0.5f);

	for (int y = yStart; y < yEnd; y++)
	{
		// calculate start and end points(x coord)
		const float px0 = m0*((float)y + 0.5f - v0->m128_f32[1]) + v0->m128_f32[0];
		const float px1 = m1*((float)y + 0.5f - v0->m128_f32[1]) + v0->m128_f32[0];

		// start and end pixels
		const int xStart = (int)ceil(px0 - 0.5f);
		const int xEnd = (int)ceil(px1 - 0.5f);

		CHAR_INFO r;
		r.Attributes = col;
		r.Char.UnicodeChar = c;
		DWord value = *(DWord*)&r;

		if(xEnd - xStart)
			memset32(self->localFrame+y*self->nFrameLength + xStart, value, xEnd - xStart);
	}
}
void _DrawTriangle(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2, wchar_t c, Word col)
{
	if (v1->m128_f32[1] < v0->m128_f32[1]) swapptr(&v0, &v1);
	if (v2->m128_f32[1] < v1->m128_f32[1]) swapptr(&v1, &v2);
	if (v1->m128_f32[1] < v0->m128_f32[1]) swapptr(&v0, &v1);

	if (v0->m128_f32[1] == v1->m128_f32[1])	// Flat Top
	{
		if (v1->m128_f32[0] < v0->m128_f32[0]) swapptr(&v0, &v1);
		//DrawFlatTopTriangle(self, v0, v1, v2, c, col);
	}
	if (v1->m128_f32[1] == v2->m128_f32[1])	// Flat Bottom
	{
		if (v2->m128_f32[0] < v1->m128_f32[0]) swapptr(&v1, &v2);
		DrawFlatBottomTriangle(self, v0, v1, v2, c, col);
	}
	else // General
	{
		const float alphaSplit =
			(v1->m128_f32[1] - v0->m128_f32[1]) / (v2->m128_f32[1] - v0->m128_f32[1]);

		const VMVECTOR vi = VMVectorAdd(*v0, VMVectorScale(VMVectorSubtract(*v2, *v0), alphaSplit));

		if (v1->m128_f32[0] < vi.m128_f32[0])
		{
			DrawFlatBottomTriangle(self, v0, v1, &vi, c, col);
			DrawFlatTopTriangle(self, v1, &vi, v2, c, col);
		}
		else
		{
			DrawFlatBottomTriangle(self, v0, &vi, v1, c, col);
			DrawFlatTopTriangle(self, &vi, v1, v2, c, col);
		}
	}
}
void _ClearFrame(selfptr, wchar_t c, Word col)
{
	CHAR_INFO r;
	r.Attributes = col;
	r.Char.UnicodeChar = c;
	
	DWord value = *(DWord*)&r;
	memset32(self->localFrame, value, self->nFrameHeight*self->nFrameLength);
}

constructMethodTable( 
	.ClearFrame = _ClearFrame,
	.DrawTriangleWireframe = _DrawTriangleWireframe,
	.DrawTriangle = _DrawTriangle,
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