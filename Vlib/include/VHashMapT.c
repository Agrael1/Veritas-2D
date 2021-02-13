#pragma pop_macro("HASHMAP_T")
#ifdef HASHMAP_T
#undef c_class
#define c_class HashMap(HASHMAP_T)

extern inline void Template(_Destroy_single)(HASHMAP_T* element);

void Constructor(selfptr, size_t Buckets)
{
    ALLOC_CHECK(*self = calloc(1, sizeof(**self) + Buckets * sizeof(void*)));   //hash table + number of buckets
    (*self)->bucket_cnt = Buckets;
}
void Destructor(selfptr)
{
    Template(clear)(self);
    free(*self);
}

#ifndef __HASH__F
#define __HASH__F
size_t hashFunction(const char* key)
{
    uint32_t hash = 0;
    for (const unsigned char* tmp = (const unsigned char*)key; *tmp != '\0'; tmp++)
    {
        hash = hash * 94567 + *tmp;
    }
    return hash;
}
#endif // !__HASH__F

HASHMAP_T* Template(emplace)(selfptr, StringView key)
{
    size_t value = hashFunction(key.data);
    uint32_t idx = value % (*self)->bucket_cnt;

    //alloc new item
    Template(hash_item)* new_item = malloc(sizeof(*new_item));
    ALLOC_CHECK(new_item);

    new_item->next = (*self)->bucket[idx];
    String_ctor(&new_item->key, key.data, key.size);
    
    (*self)->bucket[idx] = new_item;
    (*self)->size++;
    return &new_item->value;
}
HASHMAP_T* Template(find)(selfptr, const String* key)
{
    size_t hash = hashFunction(c_str(key));
    uint32_t idx = hash % (*self)->bucket_cnt;
    Template(hash_item)* tmp = (*self)->bucket[idx];   //save current item

    while (tmp != NULL)
    {
        if (string_cmp(key, &tmp->key))
        {
            return &tmp->value;
        }
        tmp = tmp->next;
    }

    return NULL;
}
NODISCARD HASHMAP_T* Template(cfind)(selfptr, const char* key)
{
    size_t hash = hashFunction(key);
    uint32_t idx = hash % (*self)->bucket_cnt;
    Template(hash_item)* tmp = (*self)->bucket[idx];   //save current item

    while (tmp != NULL)
    {
        if (strcmp(key, c_str(&tmp->key)) == 0)
        {
            return &tmp->value;
        }
        tmp = tmp->next;
    }

    return NULL;
}
void Template(clear)(selfptr)
{
    Template(hash_item)** curr_item = (*self)->bucket;
    Template(hash_item)* free_item;

    //go through the list
    for (uint32_t i = 0; i < (*self)->bucket_cnt && (*self)->size; i++)
    {
        free_item = *curr_item;
        while (free_item)
        {
            Template(hash_item)* tmp = free_item->next;
            String_dtor(&free_item->key);
            Template(_Destroy_single)(&free_item->value);
            free(free_item);
            (*self)->size--;
            free_item = tmp;
        }
        *curr_item = NULL;
        curr_item++;
    }
}
void Template(erase)(selfptr, StringView key)
{
    size_t hash = hashFunction(key);
    uint32_t idx = hash % (*self)->bucket_cnt;
    Template(hash_item)* tmp = (*self)->bucket[idx];   //save current item

    if (!tmp) return;

    if (strcmp(key.data, c_str(&tmp->key) == 0)
    {
        String_dtor(&tmp->key);
        Template(_Destroy_single)(&tmp->value);
        (*self)->bucket[idx] = tmp->next;
        free(tmp);
        (*self)->size--;
        return;
    }

    Template(hash_item)* free_item = NULL;
    while (tmp->next)
    {
        free_item = tmp->next;
        if (strcmp(key.data, c_str(&free_item->key) == 0)
        {
            String_dtor(&free_item->key);
            Template(_Destroy_single)(&free_item->value);
            tmp->next = free_item->next;
            (*self)->size--;
            free(free_item);
            return;
        }
        tmp = tmp->next;
    }
}

#undef HASHMAP_T
#include "VHashMapT.c"
#endif
