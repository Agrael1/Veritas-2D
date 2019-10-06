#include "Color.h"
#include "VeritasMath.h"
#include "GouraudVST.h"
#include "GouraudPST.h"
#include "Class.h"

extern cBuf_GouraudVST GVSCB;


CHAR_INFO virtual(Apply)(void* self, void* _in)
{
	account(self);
	ColorMap* Diffuse = &base.TextureBuffer[0];
	VMFLOAT2A* Coord = (VMVECTOR*)_in + 2;
	//SVMVECTOR Sample = { .v = VMVectorMultiply(((VMVECTOR*)_in)[2], VMLoadFloat2(&Diffuse->Width)) };
	//UINT y = (UINT)Sample.c.y * Diffuse->Stride + (UINT)Sample.c.x;
	UINT y = (UINT)(Coord->y*Diffuse->Height)*Diffuse->Stride + (UINT)(Coord->x*Diffuse->Width);
	return Pick(Diffuse->map[y].Attributes, (Byte)(_mm_cvtss_f32(((VMVECTOR*)_in)[1]) * 4.0f), FG_Moon_White);
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
