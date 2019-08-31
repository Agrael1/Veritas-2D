#include "Color.h"
#include "VeritasMath.h"
#include "GouraudVST.h"
#include "GouraudPST.h"
#include "Class.h"

extern cBuf_GouraudVST GVSCB;

CHAR_INFO virtual(Apply)(void* self, void* _in)
{
	account(self);
	UINT y = (UINT)(((SVMVECTOR*)_in + 2)->c.y*9.0f) * 9 + (UINT)(((SVMVECTOR*)_in + 2)->c.x*9.0f);
	return Pick(base.textureBuf[y]->Attributes, (Byte)(_mm_cvtss_f32(((VMVECTOR*)_in)[1]) * 4.0f), GVSCB.diffuse);
}

Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Apply = virtual(Apply);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC
