// Copyright (c) 2020
 //      Ilya Doroshenko, xdoros01
 //      agrael@live.ru, xdoros01@fit.vutbr.cz
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

#ifdef OPTIONAL_T
#include <stdbool.h>
#include "RuntimeClass.h"
#undef c_class
#define c_class Optional(OPTIONAL_T)
#pragma push_macro("OPTIONAL_T")

#define Optional(T__) __rcat2(Optional,T__)
#define unique_optional(T__) UNIQUE(Optional(T__))
#define nullopt(type) ((Optional(type)){0})

#ifndef TC
/// @brief Destroys the contents if valid and IsClass is set
inline void Destructor(selfptr)
{
#ifdef IsClass
	if(bValid) __rdtor(OPTIONAL_T)(&self->Value);
#endif
}

/// @brief Optional structure
typedef struct c_class
{
	OPTIONAL_T Value; ///<A class or type to be stored
	bool bValid; ///<Validator, if 0 -> do not look at the contents
}c_class;
#endif //TC

#undef IsClass
#undef OPTIONAL_T
#endif // OPTIONAL_T