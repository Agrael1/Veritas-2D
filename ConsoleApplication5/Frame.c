#include "Frame.h"
#include "Class.h"
#include <malloc.h>

void _Clip(void* self, Word *x, Word *y)
{
	struct c_class* this = self;
	if (*x > this->nFrameLength) *x = this->nFrameLength;
	if (*y > this->nFrameHeight) *y = this->nFrameHeight;
}
void _PrintFrame(void* self, Word x, Word y, wchar_t character, Word color)
{
	struct c_class* this = self;
	this->localFrame[y*this->nFrameLength + x].Char.UnicodeChar = character;
	this->localFrame[y*this->nFrameLength + x].Attributes = color;
}
void _Fill(void* self, Word x1, Word y1, Word x2, Word y2, wchar_t sym, Word color)
{
	struct c_class* this = self;

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

	for (int j = offsetY; j<localBuffer->nFrameHeight; j++)
		for (int i = offsetX; i<localBuffer->nFrameLength; i++)
		{
			this->localFrame[j*this->nFrameLength + i] = localBuffer->localFrame[(j - offsetY)*(localBuffer->nFrameLength) + i - offsetX];
		}
}

constructMethodTable( 
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