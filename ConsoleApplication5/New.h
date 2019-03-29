// New.h
//
//		GNU GPL License 3.0
//
// Functions for creating and deleting classes.

#ifndef New_h
#define New_h
// creates instance of class with launching constructor with parameters
void* new (const void* type, ...);

// deletes a class instance
void delete(void* item);
unsigned int sizeOf(const void* _self);
#endif // !New

