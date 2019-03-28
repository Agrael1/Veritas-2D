// BitStack.h
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Class declaration file
// Stack for small bit values (<8 bit)
#include "EngineCommons.h"

#ifndef BitStack
#define BitStack

#define c_class Stack

// Access methods table
typedef struct {
	// Pushes bits into the stack and automatically pushes complete 32/64 bits to the big one
	// self - pointer to the current class pointer (this)
	// value - value to push from 0 to 255
	// length - length in bits to push < 9
	const void(*bPush)(void* self, unsigned char value, unsigned char length);
	// Pops bits from the stack and automatically pops complete 32/64 bits from the big one
	// self - pointer to the current class pointer (this)
	// length - length in bits to pop < 9
	short(*bPop)(void* self, unsigned char length);
}vftb;

extern const void* Stack;						// type definition for new()

#if _WIN64
#define NREG 64									// using data type, which fits the entire register
#define DST 50

struct Stack
{
	const void* _class;							// class description goes here
	const vftb *method;							// class function table 

	privatev(
	unsigned long long data[DST];				// storage for stack values
	);
					
	unsigned int head;							// pointer to the last element of big stack
	Byte bitctr;						// pointer to the last element of bits
};
#else
#define NREG 32
#define DST 90

struct Stack
{
	const void* class;							// class description goes here
	const vftb *method;							// class function table 

	privatev(
		unsigned int data[DST];			// storage for stack values
	);

	unsigned int head;							// pointer to the last element of big stack
	Byte bitctr;						// pointer to the last element of bits
};
#endif
#endif // !BitStack


