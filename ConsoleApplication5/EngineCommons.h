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
typedef unsigned char Byte;
typedef _Bool bool;

#define M_PI 3.14159265358979323846
#define true 1
#define false 0
#endif

#undef vftb
#undef c_class
#undef private
// Easy mangling for method and private tables 
#define __xvftb(x) x ## _vftb
#define __rvftb(x) __xvftb(x)
#define vftb __rvftb(c_class)

#define __xprivate(x) x ## _private
#define __rprivate(x) __xprivate(x)
#define _private __rprivate(c_class)

#define privatev(...) Byte __internal_prtb[ sizeof( struct _private{ __VA_ARGS__ } )]

#define private (*(struct _private*)(this->__internal_prtb))

