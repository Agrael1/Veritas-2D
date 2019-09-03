// New.h
//
//		GNU GPL License 3.0
//
// Functions for creating and deleting classes.

#ifndef New_h
#define New_h

#define delete_s(block) if(block) delete(block)

void construct(void* where, const void* what, ...);
void deconstruct(void* self);

// creates instance of class with launching constructor with parameters
void* __cdecl new (const void* type, ...);

// deletes a class instance
void delete(void* item);
unsigned int sizeOf(const void* _self);
const char* stringOf(const void* _self);
#endif // !New

