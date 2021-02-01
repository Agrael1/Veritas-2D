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

#pragma once
#include <string.h>
#include <stdbool.h>
#include "RuntimeClass.h"

#pragma push_macro("c_class")
#undef c_class
#define c_class String
#define unique_string UNIQUE(String)

typedef union String String;
typedef struct StringView StringView;

void Constructor(String* self, const char* data, size_t size);

void Destructor(String* self);

union String
{
    struct
    {
        char* data;
        size_t size;
        size_t capacity : sizeof(size_t) * 8 - 1;
        size_t flag : 1;
    };
    char altrepr[sizeof(void*) * 3];
};

struct StringView
{
    const char* data;
    size_t size;
};

inline NODISCARD String string_move(String* self)
{
    char* data = self->data;
    self->data = NULL;
    return CLTYPE(String) { { data, self->size, self->capacity, self->flag} };
}

inline void string_assign(String* self, String from) //activates for unwinder
{
    Destructor(self);
    *self = from;
}

inline void string_move_assign(String* to, String* from)
{
    string_assign(to, string_move(from));
}

NODISCARD String string_copy(const String* from);

inline void string_copy_assign(String* to, String* from)
{
    string_assign(to, string_copy(from));
}

inline NODISCARD const char* c_str(const String* self)
{
    return !self->flag ? (const char*)self : self->data;
}

inline NODISCARD size_t string_length(const String* self)
{
    return !self->flag ? sizeof(String) - 1 - self->altrepr[sizeof(String) - 1] : self->size;
}

inline char string_at(const String* self, size_t n)
{
    return (n < string_length(self)) ? c_str(self)[n] : '\0';
}

inline bool string_empty(const String* self)
{
    return !self->flag ? !self->altrepr[0] : !self->data || !self->data[0];
}

inline size_t string_capacity(const String* self)
{
    return (!self->flag) ? (size_t)self->altrepr[sizeof(String) - 1] : self->capacity;
}

inline bool string_cmp(const String* left, const String* right)
{
    return strcmp(c_str(left), c_str(right)) == 0;
}

//String View

inline NODISCARD StringView string_view(const String* self)
{
    return CLTYPE(StringView) { c_str(self), string_length(self) };
}

inline NODISCARD StringView string_view_c(const char* str)
{
    return CLTYPE(StringView) { str, strlen(str) };
}

inline NODISCARD StringView substr(StringView str, size_t start, size_t count)
{
    return CLTYPE(StringView) {
        str.data + start,
            count ? str.size - start + count : str.size - start
    };
}

/*----------API------------*/

void string_push_back(String* self, char c);

void string_clear(String* self);

void string_insert_range(String* self, size_t start, StringView from);

void string_cat(String* self, const String* from);

void string_append(String* self, const char* input);



#ifndef VSTRING_IMPL
#pragma pop_macro("c_class")
#endif