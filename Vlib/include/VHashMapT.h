#include "VString.h"
#include <stdlib.h>

#define HashMap(T__) __rcat2(HashMap_,T__)
#define unique_hash_ptr(T__) UNIQUE(HashMap(T__))


#ifdef HASHMAP_T
#undef c_class
#define c_class HashMap(HASHMAP_T)
#pragma push_macro("HASHMAP_T")

typedef struct __rcat2(_, c_class)* c_class; //unique_ptr

#ifndef TC
typedef struct Template(hash_item)
{
    String key;
    HASHMAP_T value;
    struct Template(hash_item)* next;
}Template(hash_item);

struct __rcat2(_, c_class)
{
    uint32_t size;          /**< table size */
    uint32_t bucket_cnt;    /**< size of next item */
    Template(hash_item)* bucket[]; /**< array of items in hash table (array of pointers) */
};

#ifdef IsClass
inline void Template(_Destroy_single)(HASHMAP_T* element)
{
    __rdtor(HASHMAP_T)(element);
}
#else
inline void Template(_Destroy_single)(HASHMAP_T* element)
{
    unused_param(element);
}
#endif // IsClass
#endif // TC

void Constructor(selfptr, size_t Buckets);
void Destructor(selfptr);

NODISCARD HASHMAP_T* Template(emplace)(selfptr, const String* key);
NODISCARD HASHMAP_T* Template(find)(selfptr, const String* key);
NODISCARD HASHMAP_T* Template(cfind)(selfptr, const char* key);
void Template(erase)(selfptr, StringView key);
void Template(clear)(selfptr);


#undef IsClass
#undef HASHMAP_T

#endif // T

