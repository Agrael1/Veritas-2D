#include "Color.h"
#include "VeritasMath.h"
#include "VLine.h"
#include "FlatLightGS.h"
#include "ExperimentalPS.h"
#include "Class.h"

extern IAOut ia;
extern GSOut gs;

CHAR_INFO virtual(Apply)(void* self, void* _in)
{
	return gs.color;
}

Constructor(selfptr, va_list *ap)
{
	self->Apply = virtual(Apply);
	return self;
}
ENDCLASSDESCDD