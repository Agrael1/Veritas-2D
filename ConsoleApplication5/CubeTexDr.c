#include "GouraudVST.h"
#include "GouraudPST.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "TextureBuffer.h"
#include "TransformBuffer.h"
#include "Light.h"

#include "Header.h"
#include "Box.h"
#include "CubeTexDr.h"
#include "Class.h"


VMMATRIX virtual(GetTransformXM)(const void* self)
{
	const account(self);
	VMMATRIX R1 = VMMatrixRotationRollPitchYawFromVector(VMLoadFloat3A(&this->orientation));
	VMMATRIX T = VMMatrixTranslationFromVector(VMLoadFloat3A(&this->pos));

	VMMATRIX res = VMMatrixMultiply(R1, &T);
	return res;
}

Constructor(selfptr, va_list *ap)
{
	struct c_class *this = ((struct Class*)Drawable)->ctor(self, ap);
	base.method->GetTransformXM = virtual(GetTransformXM);

	self->pos = (VMFLOAT3A) { 0.0f, 0.0f, 0.0f };
	self->orientation = (VMFLOAT3A) { 0.0f, 0.0f, 0.0f };

	base.trilist = Box_Make(sizeof(virtual(Vertex)));
	MakeIndependent(&base.trilist);
	CalcNormalsIndependentFlat(&base.trilist, 16);

	for (size_t i = 0; i < base.trilist.numVerts; i += 6)
	{
		((virtual(Vertex)*)base.trilist.vertices)[i + 0].tc = (SVMVECTOR) { 0.0f, 1.0f, 0.0f, 0.0f };
		((virtual(Vertex)*)base.trilist.vertices)[i + 1].tc = (SVMVECTOR) { 0.0f, 0.0f, 0.0f, 0.0f };
		((virtual(Vertex)*)base.trilist.vertices)[i + 2].tc = (SVMVECTOR) { 1.0f, 1.0f, 0.0f, 0.0f };
		((virtual(Vertex)*)base.trilist.vertices)[i + 3].tc = (SVMVECTOR) { 0.0f, 0.0f, 0.0f, 0.0f };
		((virtual(Vertex)*)base.trilist.vertices)[i + 4].tc = (SVMVECTOR) { 1.0f, 0.0f, 0.0f, 0.0f };
		((virtual(Vertex)*)base.trilist.vertices)[i + 5].tc = (SVMVECTOR) { 1.0f, 1.0f, 0.0f, 0.0f };
	}

	base.method->AddBind(&base, *Resolve_VertexShader((char*)stringOf(GouraudVST), GouraudVST));
	base.method->AddBind(&base, *Resolve_PixelShader((char*)stringOf(GouraudPST), GouraudPST));
	base.method->AddBind(&base, *Resolve_TextureBuffer((char*)"Tex0", tex, 0u));
	base.method->AddBind(&base, make_shared(TransformBuffer, self, 0));
	return self;
}
Destructor(selfptr)
{
	struct c_class *this = ((struct Class*)Drawable)->dtor(self);
	return self;
}
ENDCLASSDESC