#define VS_IMPL
#include "Class.h"
#include "VLine.h"
#include "VertexShader.h"

void virtual(Bind)(selfptr, struct VLine* gfx)
{
	gfx->VSApply = self->Apply;
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
extern inline shared_ptr(Bindable)* virtual(Resolve)(char* info, void* pVS);