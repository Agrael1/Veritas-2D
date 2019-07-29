#include "VeritasMath.h"
#include "ColorIndexPS.h"
#include "Class.h"
#include <memory.h>

CHAR_INFO virtual(Apply)(selfptr, size_t primID)
{
	return Pick(self->colors[(primID) % 8],2,0);
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Apply = virtual(Apply);
	memcpy(self->colors,(enum DB16_COLOUR[]){ 0x10,0xe0,0x70,0x20,0x80,0x30,0x60,0xb0 },8*sizeof(enum DB16_COLOUR));
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC