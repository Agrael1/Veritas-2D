#include "VeritasMath.h"
#include "Class.h"
#include "CubeDemo.h"

bool virtual(OnUserCreate)(void* self)
{
	account(self);
	
	this->meshCube.p = (Triangle[]){
		// Front plate
		{ 0.0f, 0.0f, 0.0f, 	0.0f, 1.0f, 0.0f,	1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f },
		// Back plate
		{ 0.0f, 0.0f, 1.0f, 	0.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f, 1.0f },
		// Top plate
		{ 0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 1.0f,	1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 1.0f, 0.0f },
		// Bottom plate
		{ 0.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 1.0f,	1.0f, 0.0f, 0.0f },
		// Right plate
		{ 1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,	1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,	1.0f, 0.0f, 1.0f },
		// Left plate
		{ 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,	0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, 0.0f },
	};
	
	// Setting up projection matrix
	base.Output->projection = VMMatrixPerspectiveLH(1.0f, (float)base.Output->nFrameHeight / (float)base.Output->nFrameLength, 0.5f, 40.0f);
	base.Output->method->DrawTriangle(base.Output, 0, 0, 40, 40, 15, 2, 0x2588, FG_CYAN);
	return true;
}
bool virtual(OnUserUpdate)(void* self)
{
	account(self);
	


	return true;
}

Constructor(void* self, va_list *ap)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->ctor(self, ap);
	base.AppName = stringOf(self);
	base.method->OnUserCreate = virtual(OnUserCreate);
	base.method->OnUserUpdate = virtual(OnUserUpdate);

	return this;
}
Destructor(void* self)
{
	struct c_class *this = ((struct Class*)VeritasEngine)->dtor(self);

	return this;
}
ENDCLASSDESC