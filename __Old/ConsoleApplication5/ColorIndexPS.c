#include "VLine.h"
#include "ColorIndexPS.h"
#include "Class.h"

extern IAOut ia;

CHAR_INFO virtual(Apply)(void* self, void* _unused)
{
	account(self);
	return Pick(this->colors[(ia.SV_PrimID) % 8],2,0);
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	self->Apply = virtual(Apply);
	memcpy(self->colors,(enum DB16_COLOUR[]){ 0x10,0xe0,0x70,0x20,0x80,0x30,0x60,0xb0 },8*sizeof(enum DB16_COLOUR));
	return self;
}
ENDCLASSDESCDD