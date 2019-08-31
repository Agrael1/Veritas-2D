#include "GouraudVST.h"
#include "GouraudPST.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "TextureBuffer.h"
#include "Light.h"

#include "Header.h"
#include "Box.h"
#include "CubeTexDr.h"
#include "Class.h"


Constructor(selfptr, va_list *ap)
{
	struct c_class *this = ((struct Class*)Drawable)->ctor(self, ap);
	
	self->light = new(DirectionalLight);

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

	base.method->AddBind(&base, *Resolve_VertexShader((char*)stringOf(GouraudVST), GouraudVST,self->light));
	base.method->AddBind(&base, *Resolve_PixelShader((char*)stringOf(GouraudPST), GouraudPST));
	base.method->AddBind(&base, *Resolve_TextureBuffer((char*)"Tex0", tex, 0u));
	return self;
}
Destructor(selfptr)
{
	struct c_class *this = ((struct Class*)Drawable)->dtor(self);
	return self;
}
ENDCLASSDESC