#define T void
#include "Shared_ptr.h"
#include <Class.h>
#include <Unwinder.h>
#include <assert.h>

typedef shared_ptr(void) shared_ptr_base, *pshared_ptr_base;
typedef struct _control_block
{
	const void* _table;
	void* data;
	uint16_t StrongRefs;
	uint16_t WeakRefs;
}_control_block;

void copy_shared(void** self, void** to)
{
	*(pshared_ptr_base)to = *(pshared_ptr_base)self;
	((pshared_ptr_base)self)->control->StrongRefs++;
	__stackvar_insert(to, true);
}

void destroy_shared(_control_block* self)
{
	if (--self->StrongRefs == 0)
	{
		__unwinder_suppress();
		deconstruct(self->data);
		__unwinder_release();
		self->data = NULL;
		if (!--self->WeakRefs)
		{
			free(self);
		}
	}	
}
void destroy_weak(weak_ptr* self)
{
	if (--((_control_block*)self->_ctrl_ptr)->WeakRefs == 0)
	{
		free(self->_ctrl_ptr);
	}
}

struct Class _shared_ptr =
{
	.dtor = destroy_shared,
	.size = make_shared,
	.ctor = NULL,
	.typestring = "shared_ptr"
};
struct Class _weak_ptr =
{
	.dtor = destroy_weak,
	.size = 0,
	.ctor = NULL,
	.typestring = "weak_ptr"
};

void make_shared(void** _where, const void* type, ...)
{
	va_list ap;
	va_start(ap, type);

	const struct Class* pclass = type;
	_control_block* p = calloc(1, pclass->size + sizeof(_control_block));
	assert(p);
	*p = (_control_block){
		._table = &_shared_ptr,
		.StrongRefs = 1,
		.WeakRefs = 1,
		.data = p + 1
	};
	p[1]._table = type;
	if (pclass->ctor)					
	{
		__unwinder_suppress();
		pclass->ctor(p + 1, &ap);				
		__unwinder_release();
	}
	va_end(ap);
	*_where = *(void**)p;
	__stackvar_insert(_where, true);
}
void attach_shared(void** _where, void** from)
{
	if(((pshared_ptr_base)_where)->control)
		destroy_shared(((pshared_ptr_base)_where)->control);
	*_where = *from;
	((pshared_ptr_base)_where)->control->StrongRefs++;
	__stackvar_insert(_where, true);
}

void weak_from(void** self, void** to)
{
	pshared_ptr_base this = (pshared_ptr_base)self;
	weak_ptr* _to = (weak_ptr*)to;
	_to->_class = &_weak_ptr;
	_to->_ctrl_ptr = this->control;
	this->control->WeakRefs++;
	__stackvar_insert(to, true);
}

void* _weak_lock_unsafe(weak_ptr pWeak)
{
	_control_block* pCtrl = pWeak._ctrl_ptr;
	if (pCtrl->data)
		return pCtrl->data;
	return NULL;
}