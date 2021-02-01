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

/// @brief Constructs the string with optionally provided input string
/// @param [uninit]	self - pointer to a newly created string
/// @param [in,opt]	data - initial string
/// @param [opt]	size - size of input string (if 0 is scanned automatically)
void Constructor(String* self, const char* data, size_t size);

/// @brief Destroys the object, freeing malloc if long
/// @param self - pointer to initialized string
void Destructor(String* self);

/// @brief Do not use contents directly
union String
{
    /// @brief Long String representation
    struct
    {
        char* data; ///<Pointer to allocated data
        size_t size;    ///<String length 
        size_t capacity : sizeof(size_t) * 8 - 1; ///<String container size
        size_t flag : 1; ///< short or long representation
    };
    char altrepr[sizeof(void*) * 3]; ///<Short string
};

/// @brief View over string, used for function arguments
/// Better than raw string
struct StringView
{
    const char* data; ///<String container
    size_t size; ///<Length of string
};

/// @brief Creates a new string by moving data from the old one (old gets invalidated, but destroyable) 
/// Can be passed to the destructor and will work correctly afterwards 
/// The only reason it's here because of long strings, that should not be copied.
/// @param [invalidated] self - pointer to a valid string 
/// @return [nodiscard] a new string object, must be placed to an uninitialized container immediately
inline NODISCARD String string_move(String* self)
{
    char* data = self->data;
    self->data = NULL;
    return CLTYPE(String) { { data, self->size, self->capacity, self->flag} };
}

/// @brief Assignment operator
/// @param self - pointer to a valid string(gets correctly initialized)
/// @param from - copy value 
inline void string_assign(String* self, String from) //activates for unwinder
{
    Destructor(self);
    *self = from;
}

/// @brief Move from one initialized string to another, performing destruction on 'to'
/// @param to	- gets destroyed and reinitialized 
/// @param from	- gets emptied
inline void string_move_assign(String* to, String* from)
{
    string_assign(to, string_move(from));
}

/// @brief Copy constructor from another object
/// @param from - gets copied
/// @return [nodiscard] a new string object, must be placed to an uninitialized container immediately
NODISCARD String string_copy(const String* from);

/// @brief Copy assign operator, performing destruction on 'to' and copying contents of 'from'
/// @param to - gets destroyed and reinitialized
/// @param from - gets copied
inline void string_copy_assign(String* to, String* from)
{
    string_assign(to, string_copy(from));
}

/// @brief Get a constant char string
/// @param self - char string from
/// @return Const char array
inline NODISCARD const char* c_str(const String* self)
{
    return !self->flag ? (const char*)self : self->data;
}

/// @brief Get a length of string
/// @param self - pointer to a valid string 
/// @return length in bytes
inline NODISCARD size_t string_length(const String* self)
{
    return !self->flag ? sizeof(String) - 1 - self->altrepr[sizeof(String) - 1] : self->size;
}

/// @brief Get a character at position n
/// @param self - pointer to a valid string
/// @param n - position 
/// @return resulting character from position [n]
inline char string_at(const String* self, size_t n)
{
    return (n < string_length(self)) ? c_str(self)[n] : '\0';
}

/// @brief Empty check
/// @param self - pointer to a valid string
/// @return true if empty
inline bool string_empty(const String* self)
{
    return !self->flag ? !self->altrepr[0] : !self->data || !self->data[0];
}

/// @brief How much can a container hold before reallocation
/// @param self - pointer to a valid string
/// @return size of container
inline size_t string_capacity(const String* self)
{
    return (!self->flag) ? (size_t)self->altrepr[sizeof(String) - 1] : self->capacity;
}

/// @brief Compares two strings
/// @param left 
/// @param right 
/// @return true if strings are equal
inline bool string_cmp(const String* left, const String* right)
{
    return strcmp(c_str(left), c_str(right)) == 0;
}

//String View

/// @brief Constructs a view from string
/// @param self - pointer to a valid string
/// @return constant view over string
inline NODISCARD StringView string_view(const String* self)
{
    return CLTYPE(StringView) { c_str(self), string_length(self) };
}

/// @brief Constructs a view from char string
/// @param str - pointer to null terminated string
/// @return constant view over string
inline NODISCARD StringView string_view_c(const char* str)
{
    return CLTYPE(StringView) { str, strlen(str) };
}

/// @brief Get a substring view
/// Note: substring is physically taken as an offset from the beginning
/// but the functions can also use 'size' field to correctly work with substrings
/// @param str - view to take substring from
/// @param start - offset from the beginning
/// @param count - how many chars does it contain, if 0 then all until the end
/// @return constant view over string
inline NODISCARD StringView substr(StringView str, size_t start, size_t count)
{
    return CLTYPE(StringView) {
        str.data + start,
            count ? str.size - start + count : str.size - start
    };
}

/*----------API------------*/

/// @brief Append a character, array is automatically extended if needed
/// @param self - pointer to a valid string
/// @param c - character to put
void string_push_back(String* self, char c);

/// @brief Clear a string, capacity stays the same
/// @param self - pointer to a valid string
void string_clear(String* self);

/// @brief Inserts a part of a 'from' string into 'self' erases contents after start!
/// @param self - string to write to
/// @param start - after which symbol to write
/// @param from - string to write from
void string_insert_range(String* self, size_t start, StringView from);

/// @brief 
/// @param self - string to write to
/// @param from - string to write from
void string_cat(String* self, const String* from);

/// @brief 
/// @param self - string to write to
/// @param input - string to write from
void string_append(String* self, const char* input);



#ifndef VSTRING_IMPL
#pragma pop_macro("c_class")
#endif