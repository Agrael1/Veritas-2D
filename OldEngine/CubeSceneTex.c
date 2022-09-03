#include "GouraudPST.h"
#include "GouraudVST.h"
#include "Box.h"
#include "Header.h"
#include "CubeSceneTex.h"
#include "Class.h"


Constructor(selfptr, va_list *ap)
{
	struct c_class* this = ((struct Class*)TestObject)->ctor(self, ap);

	float scale = (float)va_arg(*ap, double);

	this->model = Box_Make(sizeof(virtual(Vertex)));
	MakeIndependent(&this->model);
	CalcNormalsIndependentFlat(&this->model, 16);

	for (UINT i = 0; i < this->model.numVerts; i += 6)
	{
		((virtual(Vertex)*)this->model.vertices)[i + 0].tc = (SVMVECTOR) { 0.0f, 1.0f, 0.0f, 0.0f };
		((virtual(Vertex)*)this->model.vertices)[i + 1].tc = (SVMVECTOR) { 0.0f, 0.0f, 0.0f, 0.0f };
		((virtual(Vertex)*)this->model.vertices)[i + 2].tc = (SVMVECTOR) { 1.0f, 1.0f, 0.0f, 0.0f };
		((virtual(Vertex)*)this->model.vertices)[i + 3].tc = (SVMVECTOR) { 0.0f, 0.0f, 0.0f, 0.0f };
		((virtual(Vertex)*)this->model.vertices)[i + 4].tc = (SVMVECTOR) { 1.0f, 0.0f, 0.0f, 0.0f };
		((virtual(Vertex)*)this->model.vertices)[i + 5].tc = (SVMVECTOR) { 1.0f, 1.0f, 0.0f, 0.0f };
	}

	this->VS = new(GouraudVST);
	this->PS = new(GouraudPST);
	this->PS->texBuffer = tex;
	return this;
}
Destructor(selfptr)
{
	struct c_class* this = ((struct Class*)TestObject)->dtor(self);
	delete(this->VS);
	delete(this->PS);
	return this;
}
ENDCLASSDESC
