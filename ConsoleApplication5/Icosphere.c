#include "Icosphere.h"
#include "Class.h"

Constructor(selfptr, va_list *ap)
{
	struct c_class* this = ((struct Class*)TestObject)->ctor(self, ap);
	this->model = Icosahedron_Make(sizeof(Vertex_Icosphere));
	this->VS = new(DefaultVS);
	return this;
}
Destructor(selfptr)
{
	struct c_class* this = ((struct Class*)TestObject)->dtor(self);
	return self;
}
ENDCLASSDESC