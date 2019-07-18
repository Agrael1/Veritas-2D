#include "Color.h"
#include "VeritasMath.h"
#include "ExperimentalPS.h"
#include "Class.h"

CHAR_INFO virtual(Apply)(selfptr, VMFLOAT3 normal)
{

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