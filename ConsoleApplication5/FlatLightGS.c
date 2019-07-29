#include "Color.h"
#include "VeritasMath.h"
#include "FlatLightGS.h"
#include "Class.h"

extern IAOut ia;
GSOut gs;

void virtual(Apply)(selfptr, void* v0, void* v1, void* v2)
{
	gs.color = Pick(BG_Moon_White, ia.SV_PrimID%8, FG_Dark_Blue);
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Apply = virtual(Apply);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC