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

bool virtual(IsPress)(selfptr)
{
	return private.type == Press;
}
bool virtual(IsRelease)(selfptr)
{
	return private.type == Release;
}
bool virtual(IsInvalid)(selfptr)
{
	return private.type == Invalid;
}
Byte virtual(GetCode)(selfptr)
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
	return private.KeyStates->method->IsSet(private.KeyStates, keycode);
}
struct KeyboardEvent* virtual(ReadKey)(selfptr)
{
	if (private.KeyBuffer->Contains > 0u)
	{
		struct KeyboardEvent *ep = NULL;
		private.KeyBuffer->method->pop(private.KeyBuffer, &ep);
		return ep;
	}
	else
	{
		return new(KeyboardEvent,Invalid,0);
	}
}
bool virtual(KeyIsEmpty)(const selfptr)
{
	return private.KeyBuffer->method->empty(private.KeyBuffer);
}
void virtual(ClearKey)(selfptr)
{
	private.KeyBuffer->method->clear(private.KeyBuffer);
}

// CharRoutines
//Byte(*ReadChar)(struct c_class* self);
//bool(*CharIsEmpty)(const struct c_class* self);
//void(*ClearChar)(struct c_class* self);
//void(*Flush)(struct c_class* self);
// Internal

void _OnKeyPressed(selfptr, Byte keycode)
{
	TrimBuffer(private.KeyBuffer);
	private.KeyStates->method->Set(private.KeyStates, keycode);
	private.KeyBuffer->method->push(private.KeyBuffer, new(KeyboardEvent, Press, keycode));
}
void _OnKeyReleased(selfptr, Byte keycode)
{
	TrimBuffer(private.KeyBuffer);
	private.KeyStates->method->Reset(private.KeyStates, keycode);
	private.KeyBuffer->method->push(private.KeyBuffer, new(KeyboardEvent, Release, keycode));
}
//void(*OnChar)(char character);
void _ClearState(selfptr)
{
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

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);
	private.KeyBuffer = new(Queue, sizeof(void*), bufferSize);
	private.KeyStates = new(BitField, nKeys, nKeys);
	return self;
}
Destructor(selfptr)
{
	ClearBuffer(private.KeyBuffer);
	delete(private.KeyBuffer);
	delete(private.KeyStates);
	return self;
}
ENDCLASSDESC