#include "Tri.h"
#include "GouraudVS.h"
#include "GouraudPS.h"
#include "Triangle.h"
#include "Class.h"
#include <stddef.h>

Constructor(selfptr, va_list *ap)
{
	struct c_class* this = ((struct Class*)TestObject)->ctor(self, ap);

	this->model = Tri_Make(sizeof(virtual(Vertex)));
	CalcNormalsIndependentFlat(&this->model, offsetof(virtual(Vertex), n));

	this->VS = new(GouraudVS);
	this->PS = new(GouraudPS);
	return this;
}
Destructor(selfptr)
{
	struct c_class* this = ((struct Class*)TestObject)->dtor(self);
	delete(this->VS);
	delete(this->PS);
	return self;
}
ENDCLASSDESC