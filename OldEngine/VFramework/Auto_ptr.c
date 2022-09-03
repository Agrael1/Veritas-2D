#include "Auto_ptr.h"
#include "New.h"
#include "Unwinder.h"

void make_auto(void** _where, const void* type, ...)
{
	va_list ap;
	va_start(ap, type);
	*_where = _vnew(type, ap);
	va_end(ap);
	__stackvar_insert(_where, true);
}
