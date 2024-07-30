#define PS_IMPL
#include "VLine.h"
#include "Class.h"
#include "PixelShader.h"


void virtual(Bind)(selfptr, struct VLine* gfx)
{
	gfx->PSApply = self->Apply;
}
VirtualTable{
	.Bind = virtual(Bind)
};
Constructor(selfptr, va_list *ap)
{
	InitializeVtable(self);
	self->Apply = va_arg(*ap, void*);
}
ENDCLASSDESCDD

extern inline String virtual(GenerateUID)(char* info);
extern inline shared_ptr(Bindable)* virtual(Resolve)(char* info, void* pPS);