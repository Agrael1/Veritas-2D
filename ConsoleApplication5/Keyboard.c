#include "Keyboard.h"
#include "Class.h"

#define nKeys 256u
#define bufferSize 16u

bool virtual(KeyPressed)(const selfptr, Byte keycode)
{
	return self->KeyStates->method->IsSet(self->KeyStates, keycode);
}
KeyboardEvent* virtual(ReadKey)(selfptr)
{
	if (self->KeyBuffer.Contains == 0)
		return nullptr;
	return self->KeyBuffer.method->Read(&self->KeyBuffer);
}
void virtual(ClearKey)(selfptr)
{
	self->KeyBuffer.method->Wipe(&self->KeyBuffer);
}

// CharRoutines
char _ReadChar(selfptr)
{
	return *(char*)self->CharBuffer.method->Read(&self->CharBuffer);
}
void _ClearChar(selfptr)
{
	self->CharBuffer.method->Wipe(&self->CharBuffer);
}
void _Flush(selfptr)
{
	_ClearChar(self);
	virtual(ClearKey)(self);
}

// Internal
void _OnKeyPressed(selfptr, Byte keycode)
{
	self->KeyStates->method->Set(self->KeyStates, keycode);
	KeyboardEvent* _p = self->KeyBuffer.method->GetNext(&self->KeyBuffer);
	_p->type = Press;
	_p->code = keycode;
}
void _OnKeyReleased(selfptr, Byte keycode)
{
	self->KeyStates->method->Reset(self->KeyStates, keycode);
	KeyboardEvent* _p = self->KeyBuffer.method->GetNext(&self->KeyBuffer);
	_p->type = Release;
	_p->code = keycode;
}
void _OnChar(selfptr, char character)
{
	char* _p = self->CharBuffer.method->GetNext(&self->KeyBuffer);
	*_p = character;
}
void _ClearState(selfptr)
{
	self->KeyStates->method->FullReset(self->KeyStates);
}

constructMethodTable(
	.KeyPressed = virtual(KeyPressed),
	.ReadKey = virtual(ReadKey),
	.ClearKey = virtual(ClearKey),

	.ReadChar = _ReadChar,
	.ClearChar = _ClearChar,
	.Flush = _Flush,

	.OnKeyPressed = _OnKeyPressed,
	.OnKeyReleased = _OnKeyReleased,
	.ClearState = _ClearState
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	self->KeyStates = new(BitField, nKeys, nKeys);
	construct(&self->KeyBuffer, EventQueue, sizeof(KeyboardEvent), bufferSize, sizeof(enum Type));
	construct(&self->CharBuffer, EventQueue, sizeof(char), bufferSize, sizeof(char));
	return self;
}
Destructor(selfptr)
{
	delete(self->KeyStates);
	deconstruct(&self->KeyBuffer);
	deconstruct(&self->CharBuffer);
	return self;
}
ENDCLASSDESC