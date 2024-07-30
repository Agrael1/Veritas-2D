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

#include <vlib/string.h>
#include <stdlib.h>

#define HashMap(T__) __rcat2(HashMap_,T__)
#define unique_hash_ptr(T__) UNIQUE(HashMap(T__))

#ifdef HMAP_T
#undef c_class
#define c_class HashMap(HMAP_T)
#pragma push_macro("HMAP_T")

typedef struct __rcat2(_, c_class)* c_class; ///<unique_ptr template declaration

#ifndef TC

/// @brief A hash map item outline 
typedef struct Template(hash_item)
{
    String key; ///<Key gets stored as a string object
    HMAP_T value; ///<Value stored as a full object and gets destroyed if IsClass is defined
    struct Template(hash_item)* next; ///<simple list
}Template(hash_item);

/// @brief A hash map 
struct __rcat2(_, c_class)
{
    uint32_t size;          /**< table size */
    uint32_t bucket_cnt;    /**< size of next item */
    Template(hash_item)* bucket[]; /**< array of items in hash table (array of pointers) */
};

#ifdef IsClass
/// @brief If object stored is a class it should be destroyed with destructor
/// @param  element to destroy
inline void Template(_Destroy_single)(HMAP_T* element)
{
    __rdtor(HMAP_T)(element);
}
#else
inline void Template(_Destroy_single)(HMAP_T* element)
{
    unused_param(element);
}
#endif // IsClass
#endif // TC

/// @brief Creates a new unique pointer to hash map
/// @param [uninit] self - pointer to a memory where new hash map should be constructed
/// @param Buckets number of buckets to put together
void Constructor(selfptr, size_t Buckets);

/// @brief Destroys hash map as well as all the items inside
/// @param self - pointer to valid hash map to destroy
void Destructor(selfptr);

/// @brief Allocates a space for a new object and binds it to a hash map
/// Initialization has to be performed manually over the returned pointer
/// @param key - key to an object
/// @return pointer to a memory for a new object to use constructor on
NODISCARD HMAP_T* Template(emplace)(selfptr, StringView key);

/// @brief Searches a value mapped to a key provided
/// @param key - key to an object
/// @return valid pointer if the object is found, NULL if not
NODISCARD HMAP_T* Template(find)(selfptr, StringView key);

/// @brief Erases an element with a specific key
/// @param key - key to an object 
void Template(erase)(selfptr, StringView key);

/// @brief Clears out entire hash map, destroying its elements, but not destroing hash map itself
void Template(clear)(selfptr);


#undef IsClass
#undef HMAP_T

#endif // T

