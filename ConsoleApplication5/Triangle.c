#include "Tri.h"
#include "DefaultVS.h"
#include "FlatLightGS.h"
#include "ExperimentalPS.h"
#include "Triangle.h"
#include "Class.h"
#include <stddef.h>

Constructor(selfptr, va_list *ap)
{
	struct c_class* this = ((struct Class*)TestObject)->ctor(self, ap);

	this->model = Tri_Make(sizeof(virtual(Vertex)));
	CalcNormalsIndependentFlat(&this->model, offsetof(virtual(Vertex), n));

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
	delete(this->PS);
	return self;
}
ENDCLASSDESC