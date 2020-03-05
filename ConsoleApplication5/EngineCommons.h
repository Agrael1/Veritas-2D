// EngineCommons.h
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Main Inclusion file for all the classes in VeritasEngine project. 
// Defines all the crucial automation for class mechanics in ANSI-C and some data types
// Designed for multiple inclusions, does not consume memory, contains only compiler instructions.
// Usage: include last to every class declaration header.
#ifndef ENGINECOMMONS_H
#define ENGINECOMMONS_H

#include "New.h"
#include <VEngineTypes.h>

#pragma region Automation
#define alignas(x) __declspec(align(x))
#define alignof(x) __alignof(x)

// Easy mangling for method and private tables
#define __rclass(x) extern const void* x; struct x
#define class __rclass(c_class)

#define __xctab(x) _ ## x
#define __rctab(x) __xctab(x)

#define __xconcat(x,y) x##y
#define __rconcat(x,y) __xconcat(x,y)
#define concat3(x,y,z) __rconcat(x,__rconcat(y, z))

#define ctab __rconcat(_, c_class)
#define virtual(x) __rconcat( x, __rctab(c_class))

// construction specific
#define Destructor void* __rconcat(c_class,_dtor)
#define Constructor void* __rconcat(c_class,_ctor)
#define Allocator size_t __rconcat(c_class,_ator)

#define meth __rconcat(c_class, _method)
#define _private __rconcat(c_class,_private)
#define vftb __rconcat(c_class,_vftb)

// Type string
#define __xtypestr(x) #x
#define __rtypestr(x) __xtypestr(x)
#pragma endregion

#define inherits(x) struct x

#define account(x) struct c_class *this = x

// Private Handling
#define privatev(...)  alignas(8) Byte virtual(__internal_prtb)[ sizeof( struct _private{ __VA_ARGS__ } )]
#define private (*(struct _private*)(self->virtual(__internal_prtb)))
// Method Handling
#define methods(...) struct vftb { __VA_ARGS__ }*method
#define constructMethodTable(...) struct vftb meth = { __VA_ARGS__ }

#define VirtualTable struct vftb meth = 
#define assignMethodTable(x) ((struct c_class *)(x))->method = &meth
#define override(vmethod) this->method->vmethod = virtual(vmethod)

// Class construction handling
#define ENDCLASSDESC const struct Class ctab = { sizeof(struct c_class),\
.ator = NULL, .ctor = __rconcat(c_class,_ctor),.dtor = __rconcat(c_class,_dtor),.typestring = __rtypestr(c_class)}; \
const void* c_class = &ctab;

#define ENDCLASSDESCWA const struct Class ctab = { sizeof(struct c_class),\
.ator = __rconcat(c_class,_ator), .ctor = __rconcat(c_class,_ctor),.dtor = __rconcat(c_class,_dtor),.typestring = __rtypestr(c_class)}; \
const void* c_class = &ctab;

#define selfptr struct c_class* self

#define operator(x) virtual(x)
#define GENERATED_DESC const void* _class;
#define VTHREAD __stdcall
#define delete_s(block) if(block) delete(block)
#endif

#undef c_class

 



