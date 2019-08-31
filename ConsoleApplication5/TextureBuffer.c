#include "PSBase.h"
#include "TextureBuffer.h"
#include "Class.h"

void virtual(Bind)(void* self, struct VLine* gfx)
{
	account(self);
	gfx->PS->textureBuf[this->slot] = this->Texture;
}
Constructor(selfptr, va_list *ap)
{
	account(self);
	base.Bind = virtual(Bind);
	this->Texture = va_arg(*ap, void*);
	this->slot = va_arg(*ap, unsigned);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC