#pragma once
#include <RuntimeClass.h>
#include <VBitSet.h>

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

#define OPTIONAL_T KeyboardEvent
#include <VOptionalT.h>

#define OPTIONAL_T char
#include <VOptionalT.h>

#pragma push_macro("c_class")
#undef c_class
#define c_class Keyboard

typedef struct Keyboard Keyboard;

#define NKEYS 256

void Constructor(selfptr);
void Destructor(selfptr);

struct Keyboard
{
	Bitset(NKEYS, KeyStates);			///<Keys map
	Ring(KeyboardEvent) KeyBuffer;		///<Key event queue
	Ring(char) CharBuffer;				///<Char pressed event queue
	bool bAutorepeat;					///<[unused] enable autorepeat of char events if key is held
};

/// @brief Checks wether some key is pressed or not
/// @param keycode - which key [from VK_ messages]
/// @return true if key is pressed
inline bool KeyPressed(const selfptr, uint8_t keycode)
{
	return bitset_at(self->KeyStates, keycode, NKEYS);
}

/// @brief Empties the event queue
inline void ClearKeys(selfptr)
{
	clear_Ring_KeyboardEvent(&self->KeyBuffer);
}

/// @brief Reads next event in key queue popping out read position
/// @return valid KeyboardEvent if there is something in queue, nullopt else
NODISCARD Optional(KeyboardEvent) ReadKey(selfptr);

// CharRoutines

/// @brief Reads next char in char queue popping out read position
/// @return valid char if there is something in queue, nullopt else
NODISCARD Optional(char)ReadChar(selfptr);

/// @brief Clears entire char queue
inline void ClearChar(selfptr)
{
	clear_Ring_char(&self->CharBuffer);
}

/// @brief Clears both queues char and key
inline void KeyboardFlush(selfptr)
{
	ClearChar(self);
	ClearKeys(self);
}

// Internal

/// @brief Places key press event on queue with keycode and sets state of key to pressed
/// @param keycode - which key has been pressed
inline void OnKeyPressed(selfptr, uint8_t keycode)
{
	*emplace_Ring_KeyboardEvent(&self->KeyBuffer) = (KeyboardEvent){ (uint8_t)Press, keycode };
	bitset_set(self->KeyStates, keycode, NKEYS);
}

/// @brief Places key release event on queue with keycode and sets state of key to released
/// @param keycode - which key has been released
inline void OnKeyReleased(selfptr, uint8_t keycode)
{
	*emplace_Ring_KeyboardEvent(&self->KeyBuffer) = (KeyboardEvent){ (uint8_t)Release, keycode };
	bitset_reset(self->KeyStates, keycode, NKEYS);
}

/// @brief Places character on char queue if char key has been pressed
/// @param character - what character is pressed
inline void OnChar(selfptr, char character)
{
	*emplace_Ring_char(&self->CharBuffer) = character;
}

/// @brief Clears state of keys (resets all to released) 
inline void ClearState(selfptr)
{
	bitset_clear(self->KeyStates, NKEYS);
}

#ifndef KEYBOARD_IMPL
#pragma pop_macro("c_class")
#endif
