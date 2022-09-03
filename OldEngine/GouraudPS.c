#include "Color.h"
#include "VeritasMath.h"
#include "GouraudVS.h"
#include "GouraudPS.h"
#include "Class.h"

extern cBuf_GouraudVS GVSCB;

CHAR_INFO virtual(Apply)(void* self, void* _in)
{
	return Pick(BG_Ditry_Gray, (Byte)(_mm_cvtss_f32(((VMVECTOR*)_in)[1]) * 4.0f), GVSCB.diffuse);
}

Constructor(selfptr, va_list *ap)
{
	self->Apply = virtual(Apply);
	return self;
}
ENDCLASSDESCDD