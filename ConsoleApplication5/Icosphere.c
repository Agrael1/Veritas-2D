#include "Icosphere.h"
#include "Class.h"

Constructor(selfptr, va_list *ap)
{
	struct c_class* this = ((struct Class*)TestObject)->ctor(self, ap);
	this->model = Icosahedron_Make(sizeof(Vertex_Icosphere));
	this->VS = new(DefaultVS);
	this->GS = new(FlatLightGS);
	this->PS = new(ExperimentalPS);
	return this;
}
Destructor(selfptr)
{
	struct c_class* this = ((struct Class*)TestObject)->dtor(self);
	delete(this->VS);
	delete(this->GS);
	return self;
}
ENDCLASSDESC