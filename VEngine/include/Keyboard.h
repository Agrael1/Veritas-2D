#pragma once
#include <RuntimeClass.h>
#include <stdbool.h>

enum Type
{
	Invalid = 0,
	Press,
	Release,
};
typedef struct 
{
	uint8_t type;
	uint8_t code;
}KeyboardEvent;

#define RING_T KeyboardEvent
#include <VRingT.h>

#define RING_T char
#include <VRingT.h>

#pragma push_macro("c_class")
#undef c_class
#define c_class Keyboard

typedef struct Keyboard Keyboard;

struct Keyboard
{
	//Bitset(256) KeyStates;
	Ring(KeyboardEvent) KeyBuffer;
	Ring(char) CharBuffer;
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
#pragma pop_macro("c_class")
#endif
