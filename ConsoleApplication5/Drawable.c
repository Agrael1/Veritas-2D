#include "VLine.h"
#include "BindableBase.h"
#include "Drawable.h"
#include "Class.h"

void AddBind(selfptr, shared_ptr bindable)
{
	shared_ptr* b = self->Binds.method->get_next(&self->Binds);
	*b = bindable;
}
void virtual(Draw)(selfptr, struct VLine* gfx)
{
	for (size_t i = 0; i < self->Binds.elements + 1; i++)
	{
		struct Bindable* B = ((shared_ptr*)self->Binds.method->get(&self->Binds, i))->pointer;
		B->Bind(B, gfx);
	}
	gfx->method->Draw(gfx, &self->trilist);
}

VirtualTable{
	.AddBind = AddBind,
	.Draw = virtual(Draw)
};
Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	construct(&self->Binds, Vector, sizeof(shared_ptr));
	return self;
}
Destructor(selfptr)
{
	for (size_t i = 0; i < self->Binds.elements + 1; i++)
	{
		shared_ptr* p = (shared_ptr*)self->Binds.method->get(&self->Binds, i);
		destroy_shared(p);
	}
	deconstruct(&self->Binds);
	return self;
}
ENDCLASSDESC