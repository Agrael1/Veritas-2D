#ifndef VDEFS_H
#define VDEFS_H

#include "New.h"
#include <stdbool.h>

// operators
#define alignas(x) __declspec(align(x))
#define alignof(x) __alignof(x)

#define __xconcat(x,y) x##y
#define __rconcat(x,y) __xconcat(x,y)
#define concat3(x,y,z) __rconcat(x,__rconcat(y, z))
#define concat4(x,y,z,t) __rconcat(x,concat3(y, z, t))

// Type string
#define __xtypestr(x) #x
#define __rtypestr(x) __xtypestr(x)

#define selfptr struct c_class* self

#define class const void* c_class;\
struct __rconcat(c_class,_base)

#define inherit(mclass) struct __rconcat(mclass, _base)
#define extends(mclass) struct mclass
#define interface struct __rconcat(c_class, _vtbl)
#define implements(mclass) struct __rconcat(mclass, _vtbl)
#define ComposeType typedef struct c_class\
{\
	const void* _class;\
	struct\
	{\
		interface* method;\
	};\
	struct __rconcat(c_class,_base);\
}__rconcat(c,c_class)
#define ComposeTypeEx typedef struct c_class\
{\
	struct\
	{\
		void (*dtor)(selfptr);\
		interface;\
	}*method;\
	struct __rconcat(c_class,_base);\
}__rconcat(c,c_class)

// construction specific
#define Destructor void __rconcat(c_class,_dtor)
#define Constructor void __rconcat(c_class,_ctor)
#define VirtualTable interface __rconcat(c_class, _meth) = 
#define TMethodTable __declspec(selectany) VirtualTable
#define InitializeVtable(x) ((struct c_class *)(x))->method = &__rconcat(c_class, _meth)
#define InitializeVtableEx(x) (*(interface**)(((struct c_class *)(x))->method + 1)) = &__rconcat(c_class, _meth)

#define virtual(x) concat3( x, _, c_class)

// Private Handling
#define privatev(...)  alignas(8) unsigned char virtual(__internal_prtb)[ sizeof( struct __rconcat(c_class,_private){ __VA_ARGS__ } )]
#define private (*(struct __rconcat(c_class,_private)*)(self->virtual(__internal_prtb)))

// Class table construction handling
#define ENDCLASSDESC const struct Class __rconcat(_, c_class) = { .size = sizeof(struct c_class),\
.ctor = __rconcat(c_class,_ctor),.dtor = __rconcat(c_class,_dtor),.typestring = __rtypestr(c_class)}; \
const void* c_class = &__rconcat(_, c_class);

#define ENDCLASSDESCDD const struct Class __rconcat(_, c_class) = { .size = sizeof(struct c_class),\
.ctor = __rconcat(c_class,_ctor),.dtor = NULL,.typestring = __rtypestr(c_class)}; \
const void* c_class = &__rconcat(_, c_class);

#define ENDTEMPLATEDESCDD __declspec(selectany) const struct Class __rconcat(_, c_class) = { sizeof(struct c_class),\
.ctor = __rconcat(c_class,_ctor),.dtor = NULL,.typestring = __rtypestr(c_class)}; \
__declspec(selectany) const void* c_class = &__rconcat(_, c_class);

#endif
#undef c_class