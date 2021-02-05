// Copyright (c) 2020
//		Ilya Doroshenko, xdoros01
//		agrael@live.ru, xdoros01@fit.vutbr.cz
// All rights reserved.
//
// This code is licensed under the MIT License.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/**
 * @file RuntimeClass.h
 * @author Ilya Doroshenko
 * @brief macro definitions for class structures
 */
#pragma once
#define __xcat2(x,y)x##y
#define __rcat2(x,y) __xcat2(x,y)
#define __xcat3(x,y,z)x##y##z
#define __rcat3(x,y,z) __xcat3(x,y,z)

#define __rctor(x) __rcat3(x,_,ctor)
#define Constructor __rctor(c_class)

#define __rdtor(x) __rcat3(x,_,dtor)
#define Destructor __rdtor(c_class)

#define Implements(Interf) struct Interf* method
#define selfptr c_class* self

#define Virtual(x) Template(x)
#define VirtualTable(Interf) static const struct Interf vfptr =
#define UseVirtual(pointer) pointer->method = &vfptr
#define ALLOC_CHECK(exp) if(!(exp))exit(99)

#define Template(x) __rcat3(x,_,c_class)
#define unused_param(x) (void)(x)

#include <stdint.h>

#if defined(_MSC_VER) || (defined(__INTEL_COMPILER) && defined(_WIN32))
#if defined(_M_X64)
#define BITNESS 64
#define LONG_SIZE 4
#else
#define BITNESS 32
#define LONG_SIZE 4
#endif
#elif defined(__clang__) || defined(__INTEL_COMPILER) || defined(__GNUC__)
#if defined(__x86_64)
#define BITNESS 64
#else
#define BITNESS 32
#endif
#if __LONG_MAX__ == 2147483647L
#define LONG_SIZE 4
#else
#define LONG_SIZE 8
#endif
#endif

#if BITNESS == 64
typedef int32_t halfptr;
#else
typedef int16_t halfptr;
#endif

typedef void(*dtor_t)(void*);

#if defined(__clang__) || defined(__INTEL_COMPILER) || defined(__GNUC__)
#define UNUSED __attribute__((unused))
#define COLD __attribute__((cold))
#define NODISCARD __attribute__ ((warn_unused_result))
#define UNIQUE(x) __attribute__((cleanup(__rcat2(x,_dtor)))) x
#define DEPRECATED __attribute__((deprecated))
#else
#define UNUSED 
#define COLD
#define UNIQUE(x) x
#define NODISCARD 
#define DEPRECATED __declspec(deprecated)
#endif

#ifdef __cplusplus
#define CLTYPE(Type) 
#else
#define CLTYPE(Type) (Type)
#endif