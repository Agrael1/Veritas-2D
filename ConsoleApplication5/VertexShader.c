#include "VSBase.h"
#include "VertexShader.h"
#include "Class.h"

void virtual(Bind)(void* self, struct VLine* gfx)
{
	account(self);
	gfx->VS.Apply = this->pVertexShader.Apply;
	gfx->VS.VSOutSize = this->pVertexShader.VSOutSize;
}
Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Bind = virtual(Bind);
	construct(&self->pVertexShader, (va_arg(*ap, void*)));
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC