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
#include <limits.h>
#include <RuntimeClass.h>

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

/// @brief Move Constructor of the string
/// Clears the data from other, so it does not point to the same data
/// if other string is small, simple copy is performed
/// @param self string to construct
/// @param other string to move from
void Move(String* self, String* other);

/// @brief Copy constructor from another object
/// Performs deep copy of the container to unitialized container
/// @param self gets constructed
/// @param from - gets copied
void Copy(String* self, const String* from);

DefaultOperators

/// @brief Do not use contents directly
union String
{
    /// @brief Long String representation
    struct
    {
        char* data; ///< Pointer to allocated data
        size_t size;    ///< String length 
        size_t capacity : sizeof(size_t) * CHAR_BIT - 1; ///< String container size
        size_t large : 1; ///< Short or long representation
    };
    struct
    {
        char shortstr[sizeof(void*) * 3 - 1]; ///< Short string
        char last; ///< Last hack byte, that holds remainig capacity (terminating zero if empty)
    };
};

/// @brief View over string, used for function arguments
/// Better than raw string
struct StringView
{
    const char* data; ///<String container
    size_t size; ///<Length of string
};


/// @brief Get a constant char string
/// @param self - char string from
/// @return Const char array
inline const char* c_str(const String* self)
{
    return self->large ? self->data : (const char*)self;
}

/// @brief Get a length of string
/// @param self - pointer to a valid string 
/// @return length in bytes
inline size_t string_length(const String* self)
{
    return self->large ? self->size : sizeof(String) - 1 - self->last;
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
    return self->large ? 
        !self->data || !self->data[0] : 
        (self->last == sizeof(String) - 1);
}

/// @brief How much can a container hold before reallocation
/// @param self - pointer to a valid string
/// @return size of container
inline size_t string_capacity(const String* self)
{
    return !self->large ? 
        sizeof(String) :
        self->capacity;
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

/// @brief Concatenates strings into first one
/// @param self - string to write to
/// @param from - string to copy from
void string_cat(String* self, const String* from);

/// @brief Appends charachter stream to the end of the current string
/// @param self - string to write to
/// @param input - string to write from
void string_append(String* self, const char* input);

/// @brief Adds charachter stream before string 
/// writes bytes at the beginning without separation
/// @param self - string to write to
/// @param input - string to prepend
void string_prepend(String* self, const char* input);

/// @brief removes charachter from the front of the string
/// @param self - string 
void string_pop_front(String* self);

#ifndef VSTRING_IMPL
#pragma pop_macro("c_class")
#endif