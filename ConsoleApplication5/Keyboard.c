#include "Keyboard.h"
#include "Class.h"

#define nKeys 256u
#define bufferSize 16u

#undef c_class
#define c_class KeyboardEvent

bool virtual(IsPress)(const selfptr)
{
	return private.type == Press;
}
bool virtual(IsRelease)(const selfptr)
{
	return private.type == Release;
}
bool virtual(IsInvalid)(const selfptr)
{
	return private.type == Invalid;
}
Byte virtual(GetCode)(const selfptr)
{
	return private.code;
}

constructMethodTable(
	.IsPress = virtual(IsPress),
	.IsRelease = virtual(IsRelease),
	.IsInvalid = virtual(IsInvalid),
	.GetCode = virtual(GetCode)
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	private.type = va_arg(*ap, enum virtual(Type));
	private.code = va_arg(*ap, Byte);
	return self;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC

#undef c_class
#define c_class Keyboard

bool virtual(KeyPressed)(const selfptr, Byte keycode)
{
	return self->KeyStates->method->IsSet(self->KeyStates, keycode);
}
struct KeyboardEvent* virtual(ReadKey)(selfptr)
{
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
	void* _p = self->KeyBuffer.method->GetNext(&self->KeyBuffer);
	construct(_p, KeyboardEvent, Press, keycode);
}
void _OnKeyReleased(selfptr, Byte keycode)
{
	self->KeyStates->method->Reset(self->KeyStates, keycode);
	void* _p = self->KeyBuffer.method->GetNext(&self->KeyBuffer);
	construct(_p, KeyboardEvent, Release, keycode);
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
	construct(&self->KeyBuffer, EventQueue, sizeof(struct KeyboardEvent), bufferSize, sizeof(void*));
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