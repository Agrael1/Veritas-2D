#include "Frame.h"
#include "Class.h"
#include <math.h>
#include <malloc.h>

void _Clip(void* self, Word *x, Word *y)
{
	struct c_class* this = self;
	if (*x > this->nFrameLength) *x = this->nFrameLength;
	if (*y > this->nFrameHeight) *y = this->nFrameHeight;
}
void _PrintFrame(selfptr, Word x, Word y, wchar_t character, Word color)
{
	account(self);
	this->localFrame[y*this->nFrameLength + x].Char.UnicodeChar = character;
	this->localFrame[y*this->nFrameLength + x].Attributes = color;
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
void _DrawTriangle(selfptr, Word x1, Word y1, Word x2, Word y2, Word x3, Word y3, wchar_t c, Word col)
{
	_DrawLine(self, x1, y1, x2, y2, c, col);
	_DrawLine(self, x2, y2, x3, y3, c, col);
	_DrawLine(self, x3, y3, x1, y1, c, col);
}
constructMethodTable( 
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