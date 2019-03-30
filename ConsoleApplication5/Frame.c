#include "Frame.h"
#include "Class.h"

void ClipL(void* self, int *x, int *y)
{
	struct c_class* this = self;
	if (*x < 0) *x = 0;
	if (*x >= this->nFrameLength) *x = this->nFrameLength;
	if (*y < 0) *y = 0;
	if (*y >= this->nFrameHeight) *y = this->nFrameHeight;
}
void PrintFrameW(void* self, int x, int y, wchar_t character, short color)
{
	struct c_class* this = self;
	if (x >= 0 && x < this->nFrameLength&&y >= 0 && y < this->nFrameHeight)
	{
		this->localFrame[y*this->nFrameLength + x].Char.UnicodeChar = character;
		this->localFrame[y*this->nFrameLength + x].Attributes = color;
	}
}
void FillL(void* self, int x1, int y1, int x2, int y2, wchar_t sym, short color)
{
	struct c_class* this = self;
	ClipL(this, &x1, &y1);
	ClipL(this, &x2, &y2);
	{
		for (int x = x1; x < x2; x++)
			for (int y = y1; y < y2; y++)
			{
				this->localFrame[y*this->nFrameLength + x].Char.UnicodeChar = sym;
				this->localFrame[y*this->nFrameLength + x].Attributes = color;
			}
	}
}
void DrawRectangleL(void* self, Word x1, Word y1, Word x2, Word y2, unsigned short color)
{
	struct c_class *this = self;
	if ((x2 <= x1) && (y2 >= y1))
		FillL(this, x2, y1, x1, y2, L' ', color);
	else if ((x2 >= x1) && (y2 <= y1))
		FillL(this, x1, y2, x2, y1, L' ', color);
	else if ((x2 <= x1) && (y2 <= y1))
		FillL(this, x2, y2, x1, y1, L' ', color);
	else
		FillL(this, x1, y1, x2, y2, L' ', color);
}

constructMethodTable( .PrintFrameW = PrintFrameW, .DrawRectangle = DrawRectangleL );

void* Frame_ctor(void* self, va_list *ap)
{
	struct c_class* this = self;
	assignMethodTable(this);


	this->nFrameLength = va_arg(*ap, Word);
	this->nFrameHeight = va_arg(*ap, Word);

	this->localFrame = malloc(this->nFrameLength*this->nFrameHeight * sizeof(CHAR_INFO));

	if (this->localFrame)
		DrawRectangleL(this, 0, 0, this->nFrameLength, this->nFrameHeight, BG_BLACK);

	return this;
}

void* Frame_dtor(void* self)
{
	struct c_class* this = self;
	free(this->localFrame);
	return this;
}

const struct Class _frame = { sizeof(struct c_class) ,.ctor = Frame_ctor,.dtor = Frame_dtor };
const void* c_class = &_frame;