#include "Color.h"
#include "VeritasMath.h"
#include "FlatLightGS.h"
#include "Class.h"

CHAR_INFO virtual(Apply)(VMVECTOR normal)
{
	return Pick(BG_Moon_White, 1, FG_Dark_Blue);
}

constructMethodTable(
	.Apply = virtual(Apply)
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);

	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC