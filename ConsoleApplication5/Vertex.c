#include "Vertex.h"
#include "Class.h"



constructMethodTable(
	.x = 0
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