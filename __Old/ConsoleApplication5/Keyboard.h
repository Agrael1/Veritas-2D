#pragma once
#include "Templates.h"

enum Type
{
	Press,
	Release,
	Invalid
};

typedef struct 
{
	uint8_t type;
	uint8_t code;
}KeyboardEvent;

#define T KeyboardEvent
#define N 16
#include "RingT.h"
#undef T
#undef N

#define c_class Keyboard

class
{
	Bitset(256) KeyStates;
	Ring(KeyboardEvent, 16) KeyBuffer;
	Ring(char,16) CharBuffer;
	bool bAutorepeat;
};
interface
{
	bool (*KeyPressed)(const selfptr, uint8_t keycode);
	Optional(KeyboardEvent) (*ReadKey)(selfptr);
	void (*ClearKey)(selfptr);
	// CharRoutines
	Optional(char)(*ReadChar)(selfptr);
	void (*ClearChar)(selfptr);
	void (*Flush)(selfptr);
	// Internal
	void (*OnKeyPressed)(selfptr, uint8_t keycode);
	void (*OnKeyReleased)(selfptr, uint8_t keycode);
	void (*OnChar)(selfptr, char character);
	void (*ClearState)(selfptr);
};
ComposeTypeEx;

#ifndef KEYBOARD_IMPL
#undef c_class
#endif
