// Copyright (c) 2020
 //      Ilya Doroshenko, xdoros01
 //      agrael@live.ru, xdoros01@fit.vutbr.cz
 // All rights reserved.
 //
 // This code is licensed under the MIT License.
 //
 // Permission is hereby granted, free of charge, to any person obtaining a copy
 // of this software and associated documentation files(the "Software"), to deal
 // in the Software without restriction, including without limitation the rights
 // to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 // copies of the Software, and to permit persons to whom the Software is
 // furnished to do so, subject to the following conditions :
 //
 // The above copyright notice and this permission notice shall be included in
 // all copies or substantial portions of the Software.
 //
 // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 // THE SOFTWARE.

#pragma once
#include <vlib/bitset.h>
#include <engine/keyboard_event.h>

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
