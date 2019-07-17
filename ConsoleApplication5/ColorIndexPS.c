#include "VeritasMath.h"
#include "ColorIndexPS.h"
#include "Class.h"
#include <memory.h>

struct PSOut virtual(Apply)(selfptr, size_t primID)
{
	struct PSOut r;
	r.color = self->colors[(primID) % 8];
	r.sym = ' ';
	return r;
}

constructMethodTable(
	.Apply = virtual(Apply)
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	memcpy(self->colors,(enum DB16_COLOUR[]){ 0x10,0xe0,0x70,0x20,0x80,0x30,0x60,0xb0 },8*sizeof(enum DB16_COLOUR));
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC