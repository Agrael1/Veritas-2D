// BitStack.h
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Class declaration file
// Stack for small bit values (<8 bit)
#include "EngineCommons.h"

#ifndef BitStack_h
#define BitStack_h
#if _WIN64
#define NREG 64									// using data type, which fits the entire register
#define DST 50
#else
#define NREG 32
#define DST 90
#endif


#define c_class BitStack

class
{
	const void* _class;							// class description goes here
	privatev(
		unsigned long long data[DST];			// storage for stack values
	);
	Word head;							// pointer to the last element of big stack
	Byte bitctr;								// pointer to the last element of bits

	methods(
		// Pushes bits into the stack and automatically pushes complete 32/64 bits to the big one
		// @self - pointer to the current class pointer (this)
		// @value - value to push from 0 to 255
		// @length - length in bits to push < 9
		const void(*bPush)(void* self, Byte value, Byte length);
		// Pops bits from the stack and automatically pops complete 32/64 bits from the big one
		// @self - pointer to the current class pointer (this)
		// @length - length in bits to pop < 9
		short(*bPop)(void* self, Byte length);
	);
};
#endif // !BitStack
