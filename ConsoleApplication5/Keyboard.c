#include "Keyboard.h"
#include "Class.h"

#define nKeys 256u
#define bufferSize 16u

#undef c_class
#define c_class KeyboardEvent

void TrimBuffer(struct Queue* buffer)
{
	
	if (buffer->Contains + 1 > bufferSize)
	{
		void* _proxy = NULL;
		buffer->method->pop(buffer, &_proxy);
		delete(_proxy);
	}
}
void ClearBuffer(struct Queue* buffer)
{
	
	while (buffer->Contains)
	{
		void* _proxy = NULL;
		buffer->method->pop(buffer, &_proxy);
		delete(_proxy);
	}
}

bool virtual(IsPress)(struct c_class* self)
{
	account(self);
	return private.type == Press;
}
bool virtual(IsRelease)(struct c_class* self)
{
	account(self);
	return private.type == Release;
}
bool virtual(IsInvalid)(struct c_class* self)
{
	account(self);
	return private.type == Invalid;
}
Byte virtual(GetCode)(void* self)
{
	account(self);
	return private.code;
}

constructMethodTable(
	.IsPress = virtual(IsPress),
	.IsRelease = virtual(IsRelease),
	.IsInvalid = virtual(IsInvalid),
	.GetCode = virtual(GetCode)
);

Constructor(void* self, va_list *ap)
{
	account(self);
	assignMethodTable(this);
	private.type = va_arg(*ap, enum virtual(Type));
	private.code = va_arg(*ap, Byte);
	return this;
}
Destructor(void* self)
{
	return self;
}
ENDCLASSDESC

#undef c_class
#define c_class Keyboard

bool virtual(KeyPressed)(const void* self, Byte keycode)
{
	const account(self);
	return private.KeyStates->method->IsSet(private.KeyStates, keycode);
}
struct KeyboardEvent* virtual(ReadKey)(void* self)
{
	account(self);

	if (private.KeyBuffer->Contains > 0u)
	{
		 struct KeyboardEvent* e = NULL;
		 private.KeyBuffer->method->pop(private.KeyBuffer, &e);
		 return e;
	}
	else
	{
		return new(KeyboardEvent,Invalid,0);
	}
}
bool virtual(KeyIsEmpty)(const void* self)
{
	const account(self);
	return private.KeyBuffer->method->empty(private.KeyBuffer);
}
void virtual(ClearKey)(void* self)
{
	account(self);
	private.KeyBuffer->method->clear(private.KeyBuffer);
}

// CharRoutines
//Byte(*ReadChar)(struct c_class* self);
//bool(*CharIsEmpty)(const struct c_class* self);
//void(*ClearChar)(struct c_class* self);
//void(*Flush)(struct c_class* self);
// Internal

void _OnKeyPressed(void* self, Byte keycode)
{
	account(self);
	TrimBuffer(private.KeyBuffer);
	private.KeyStates->method->Set(private.KeyStates, keycode);
	private.KeyBuffer->method->push(private.KeyBuffer, new(KeyboardEvent, Press, keycode));
}
void _OnKeyReleased(void* self, Byte keycode)
{
	account(self);
	TrimBuffer(private.KeyBuffer);
	private.KeyStates->method->Reset(private.KeyStates, keycode);
	private.KeyBuffer->method->push(private.KeyBuffer, new(KeyboardEvent, Release, keycode));
}
//void(*OnChar)(char character);
void _ClearState(void* self)
{
	account(self);
	private.KeyStates->method->FullReset(private.KeyStates);
}

constructMethodTable(
	.KeyPressed = virtual(KeyPressed),
	.ReadKey = virtual(ReadKey),
	.KeyIsEmpty = virtual(KeyIsEmpty),
	.ClearKey = virtual(ClearKey),

	.OnKeyPressed = _OnKeyPressed,
	.OnKeyReleased = _OnKeyReleased,
	.ClearState = _ClearState
);

Constructor(void* self, va_list *ap)
{
	account(self);
	assignMethodTable(this);
	private.KeyBuffer = new(Queue, sizeof(void*), bufferSize);
	private.KeyStates = new(BitField, nKeys);
	return this;
}
Destructor(void* self)
{
	account(self);
	ClearBuffer(private.KeyBuffer);
	delete(private.KeyBuffer);
	delete(private.KeyStates);
	return this;
}
ENDCLASSDESC