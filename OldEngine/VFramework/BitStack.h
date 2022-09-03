// BitStack.h
//
//		GNU GPL License 3.0. Usage or modification is strictly welcome.
//
// Class declaration file
// Stack for small bit values (<8 bit)
#pragma once
#include "VDefs.h"
#include <stdint.h>

#if _WIN64
#define DST 50
#else
#define DST 100
#endif


#define c_class BitStack

class
{
	size_t data[DST];			// storage for stack values
	uint16_t head;				// pointer to the last element of big stack
	uint8_t bitctr;				// pointer to the last element of bits
};
interface 
{
	// Pushes bits into the stack and automatically pushes complete 32/64 bits to the big one
	// @self - pointer to the current class pointer (this)
	// @value - value to push from 0 to 255
	// @length - length in bits to push < 9
	bool(*push)(selfptr, uint8_t value, uint8_t length);
	// Pops bits from the stack and automatically pops complete 32/64 bits from the big one
	// @self - pointer to the current class pointer (this)
	// @length - length in bits to pop < 9
	short(*pop)(selfptr, uint8_t length);
	//Resets the bit stack
	void(*reset)(selfptr);
};
ComposeType;
