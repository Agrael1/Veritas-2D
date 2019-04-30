// EngineCommons.h
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Main Inclusion file for all the classes in olcGameEngine project. 
// Defines all the crucial automation for class mechanics in ANSI-C and some data types
// Designed for multiple inclusions, does not consume memory, contains only compiler instructions.
// Usage: include last to every class declaration header.
#ifndef ENGINECOMMONS_H
#define ENGINECOMMONS_H

#include "New.h"

typedef unsigned char Byte;
typedef _Bool bool;
typedef unsigned short Word;
typedef unsigned int DWord;

#if _WIN64
typedef unsigned long long MaxInt;
#else
typedef unsingned int MaxInt;
#endif

#define M_PI 3.14159265358979323846
#define true 1
#define false 0

#pragma region Automation
// Easy mangling for method and private tables
#define __rclass(x) extern const void* x; struct x
#define class __rclass(c_class)

#define __xctab(x) _ ## x
#define __rctab(x) __xctab(x)

#define __xconcat(x,y) x##y
#define __rconcat(x,y) __xconcat(x,y)

#define ctab __rconcat(_, c_class)
#define virtual(x) __rconcat( x, __rctab(c_class))
#define Destructor void* __rconcat(c_class,_dtor)
#define Constructor void* __rconcat(c_class,_ctor)
#define meth __rconcat(c_class, _method)
#define _private __rconcat(c_class,_private)
#define vftb __rconcat(c_class,_vftb)

// Type string
#define __xtypestr(x) #x
#define __rtypestr(x) __xtypestr(x)
#pragma endregion

#define inherits(x) struct x _base_ref

#define account(x) struct c_class *this = x
#define base (this->_base_ref)

// Private Handling
#define privatev(...) Byte virtual(__internal_prtb)[ sizeof( struct _private{ __VA_ARGS__ } )]
#define private (*(struct _private*)(this->virtual(__internal_prtb)))
// Method Handling
#define methods(...) struct vftb { __VA_ARGS__ }*method
#define constructMethodTable(...) struct vftb meth = { __VA_ARGS__ }
#define assignMethodTable(x) ((struct c_class *)(x))->method = &meth

// Class construction handling
#define ENDCLASSDESC const struct Class ctab = { sizeof(struct c_class),\
.ctor = __rconcat(c_class,_ctor),.dtor = __rconcat(c_class,_dtor),.typestring = __rtypestr(c_class)}; \
const void* c_class = &ctab;
#endif

#undef c_class

 



