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
void _PrintFrame(selfptr, Word x, Word y, wchar_t character, Word color)
{
	account(self);
	if (x < this->nFrameLength&&y < this->nFrameHeight)
	{
		this->localFrame[y*this->nFrameLength + x].Char.UnicodeChar = character;
		this->localFrame[y*this->nFrameLength + x].Attributes = color;
	}
}
void _Fill(selfptr, Word x1, Word y1, Word x2, Word y2, wchar_t sym, Word color)
{
	account(self);
	_Clip(this, &x1, &y1);
	_Clip(this, &x2, &y2);

	for (int x = x1; x < x2; x++)
		for (int y = y1; y < y2; y++)
			_PrintFrame(this, x, y, sym, color);
}
void _DrawRectangle(void* self, Word x1, Word y1, Word x2, Word y2, Word color)
{
	struct c_class *this = self;
	if ((x2 <= x1) && (y2 >= y1))
		_Fill(this, x2, y1, x1, y2, L' ', color);
	else if ((x2 >= x1) && (y2 <= y1))
		_Fill(this, x1, y2, x2, y1, L' ', color);
	else if ((x2 <= x1) && (y2 <= y1))
		_Fill(this, x2, y2, x1, y1, L' ', color);
	else
		_Fill(this, x1, y1, x2, y2, L' ', color);
}
void _Compose(void* self, struct Frame* localBuffer, Word offsetX, Word offsetY)
{
	struct c_class *this = self;
	_Clip(this, &offsetX, &offsetY);

	for (int j = offsetY; j<localBuffer->nFrameHeight + offsetY; j++)
		for (int i = offsetX; i<localBuffer->nFrameLength + offsetX; i++)
		{
			this->localFrame[j*this->nFrameLength + i] = localBuffer->localFrame[(j - offsetY)*(localBuffer->nFrameLength) + i - offsetX];
		}
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

		for (int x = xStart; x < xEnd; x++)
		{
			_PrintFrame(self, x, y, c, col);
		}
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

		for (int x = xStart; x < xEnd; x++)
		{
			_PrintFrame(self, x, y, c, col);
		}
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
		DrawFlatTopTriangle(self, v0, v1, v2, c, col);
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
	int value = 0;
	value |= c<<16;
	value |= col;

	memset(self->localFrame, value, self->nFrameHeight*self->nFrameLength*sizeof(CHAR_INFO));
}

constructMethodTable( 
	.ClearFrame = _ClearFrame,
	.DrawTriangleWireframe = _DrawTriangleWireframe,
	.DrawTriangle = _DrawTriangle,
	.DrawLine = _DrawLine,
	.PrintFrame = _PrintFrame,
	.DrawRectangle = _DrawRectangle,
	.Compose = _Compose
);

Constructor(void* self, va_list *ap)
{
	struct c_class* this = self;
	assignMethodTable(this);

	this->nFrameLength = va_arg(*ap, Word);
	this->nFrameHeight = va_arg(*ap, Word);

	this->localFrame = malloc(this->nFrameLength*this->nFrameHeight * sizeof(CHAR_INFO));

	if (this->localFrame)
		_DrawRectangle(this, 0, 0, this->nFrameLength, this->nFrameHeight, BG_BLACK);

	return this;
}
Destructor(void* self)
{
	struct c_class* this = self;
	free(this->localFrame);
	return this;
}
ENDCLASSDESC