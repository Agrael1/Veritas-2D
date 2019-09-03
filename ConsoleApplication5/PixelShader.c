#include "PixelShader.h"
#include "Class.h"

void virtual(Bind)(void* self, struct VLine* gfx)
{
	account(self);
	gfx->PS = this->pPixelShader;
}
Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Bind = virtual(Bind);
	self->pPixelShader = new(va_arg(*ap, void*));
	return self;
}
Destructor(selfptr)
{
	delete(self->pPixelShader);
	return self;
}
ENDCLASSDESC