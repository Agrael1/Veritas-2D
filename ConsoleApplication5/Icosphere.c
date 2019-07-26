#include "Icosphere.h"
#include "Class.h"



Constructor(selfptr, va_list *ap)
{
	struct c_class* this = ((struct Class*)TestObject)->ctor(self, ap);
	this->model = Icosahedron_MakeIndependent(sizeof(Vertex_Icosphere));
	return this;
}
Destructor(selfptr)
{
	struct c_class* this = ((struct Class*)TestObject)->dtor(self);
	return self;
}
ENDCLASSDESC