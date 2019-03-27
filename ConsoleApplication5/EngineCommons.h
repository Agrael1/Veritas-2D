// EngineCommons.h
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Main Inclusion file for all the classes in olcGameEngine project. 
// Defines all the crucial automation for class mechanics in ANSI-C and some data types
#pragma once
typedef unsigned char Byte;
typedef _Bool bool;

#define true 1
#define false 0
// Easy mangling for method and private tables 
#undef vftb
#undef c_class
#undef private

#define __xvftb(x) x ## _vftb
#define __rvftb(x) __xvftb(x)
#define vftb __rvftb(c_class)

#define __xprivate(x) x ## _private
#define __rprivate(x) __xprivate(x)
#define _private __rprivate(c_class)

#define privatev(...) Byte __internal_prtb[ sizeof( struct _private{ __VA_ARGS__ } )]

#define private (*(struct _private*)(this->__internal_prtb))