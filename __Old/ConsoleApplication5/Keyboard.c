#define KEYBOARD_IMPL
#include "Keyboard.h"

bool virtual(KeyPressed)(const selfptr, uint8_t keycode)
{
	return self->KeyStates.method->IsSet(&self->KeyStates, keycode);
}
Optional(KeyboardEvent) virtual(ReadKey)(selfptr)
{
	return self->KeyBuffer.method->pop(&self->KeyBuffer);
}
void virtual(ClearKey)(selfptr)
{
	self->KeyBuffer.method->wipe(&self->KeyBuffer);
}

// CharRoutines
Optional(char) _ReadChar(selfptr)
{
	return self->CharBuffer.method->pop(&self->CharBuffer);
}
void _ClearChar(selfptr)
{
	self->CharBuffer.method->wipe(&self->CharBuffer);
}
void _Flush(selfptr)
{
	_ClearChar(self);
	virtual(ClearKey)(self);
}

// Internal
void _OnKeyPressed(selfptr, uint8_t keycode)
{
	self->KeyStates.method->Set(&self->KeyStates, keycode);
	self->KeyBuffer.method->push(&self->KeyBuffer, (KeyboardEvent) { Press, keycode });
}
void _OnKeyReleased(selfptr, uint8_t keycode)
{
	self->KeyStates.method->Reset(&self->KeyStates, keycode);
	self->KeyBuffer.method->push(&self->KeyBuffer, (KeyboardEvent) { Release, keycode });
}
void _OnChar(selfptr, char character)
{
	self->CharBuffer.method->push(&self->CharBuffer, character);
}
void _ClearState(selfptr)
{
	self->KeyStates.method->FullReset(&self->KeyStates);
}

VirtualTable{
	.KeyPressed = virtual(KeyPressed),
	.ReadKey = virtual(ReadKey),
	.ClearKey = virtual(ClearKey),

	.ReadChar = _ReadChar,
	.ClearChar = _ClearChar,
	.Flush = _Flush,

	.OnKeyPressed = _OnKeyPressed,
	.OnKeyReleased = _OnKeyReleased,
	.OnChar = _OnChar,
	.ClearState = _ClearState
};
Constructor(selfptr, va_list *ap)
{
	InitializeVtableEx(self);
	construct(&self->KeyStates, BitsetT(256));
	construct(&self->KeyBuffer, RingT(KeyboardEvent, 16));
	construct(&self->CharBuffer, RingT(char, 16));
	self->bAutorepeat = false;
}
ENDCLASSDESCDD