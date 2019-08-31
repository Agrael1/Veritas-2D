#include "VertexShader.h"
#include "Class.h"

void virtual(Bind)(void* self, struct VLine* gfx)
{
	account(self);
	gfx->VS = this->pVertexShader;
}
Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Bind = virtual(Bind);
	self->pVertexShader = new(va_arg(*ap, void*));
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC