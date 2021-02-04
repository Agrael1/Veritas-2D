#define KEYBOARD_IMPL
#include "Keyboard.h"

extern inline bool KeyPressed(const selfptr, uint8_t keycode);
extern inline void ClearKeys(selfptr);
extern inline void ClearChar(selfptr);
extern inline void Flush(selfptr);

// Internal
extern inline void OnKeyPressed(selfptr, uint8_t keycode);
extern inline void OnKeyReleased(selfptr, uint8_t keycode);
extern inline void OnChar(selfptr, char character);
extern inline void ClearState(selfptr);

Optional(KeyboardEvent) ReadKey(selfptr)
{
	KeyboardEvent* x = top_Ring_KeyboardEvent(&self->KeyBuffer);
	if (x)
	{
		KeyboardEvent y = *x;
		pop_Ring_KeyboardEvent(&self->KeyBuffer);
		return (Optional(KeyboardEvent)) { .Value = y, .bValid = true };
	}
	return nullopt(KeyboardEvent);
}
Optional(char) ReadChar(selfptr)
{
	char* x = top_Ring_char(&self->CharBuffer);
	if (x)
	{
		char y = *x;
		pop_Ring_char(&self->CharBuffer);
		return (Optional(char)) { .Value = y, .bValid = true };
	}
	return nullopt(char);
}


void Constructor(selfptr)
{
	bitset_clear(self->KeyStates, NKEYS);
	Ring_KeyboardEvent_ctor(&self->KeyBuffer, 16);
	Ring_char_ctor(&self->CharBuffer, 16);
	self->bAutorepeat = false;
}
void Destructor(selfptr)
{
	Ring_char_dtor(&self->CharBuffer);
	Ring_KeyboardEvent_dtor(&self->KeyBuffer);
}