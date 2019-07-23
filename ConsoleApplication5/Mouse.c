#include "Exception.h"
#include "Mouse.h"
#include "Class.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

#undef c_class
#define c_class MouseEvent

// Set for future releases
enum virtual(Type) virtual(GetType)(const selfptr)
{
	const account(self);
	return private.type;
}
bool virtual(IsInvalid)(const selfptr)
{
	const account(self);
	return private.type == MInvalid;
}

constructMethodTable(
	.GetType = virtual(GetType),
	.IsInvalid = virtual(IsInvalid)
);

Constructor(selfptr, va_list* ap)
{
	account(self);
	assignMethodTable(this);
	
	private.type = va_arg(*ap, enum virtual(Type));
	private.code = va_arg(*ap, Word);

	return this;
}
Destructor(selfptr)
{
	return self;
}
ENDCLASSDESC



#undef c_class
#define c_class Mouse
#define bufferSize 16
#define nKeys 5

int virtual(GetX)(const selfptr)
{
	return self->deltaX;
}
int virtual(GetY)(const selfptr)
{
	return self->deltaY;
}


bool virtual(ButtonPressed)(selfptr, MButtons BCode)
{
	account(self);
	return private.MBStates->method->IsSet(private.MBStates, BCode);
}
void _ReadMouseMovement(selfptr, int* X, int* Y)
{
	account(self);
	if (X)
		*X = this->deltaX;
	if (Y)
		*Y = this->deltaY;
	
	this->deltaX = 0; this->deltaY = 0;
}
void _TranslateMouseInput(selfptr, RAWMOUSE* mouse)
{
	// make Lerp
	account(self);
	if (mouse->usFlags == 0)
	{
		this->deltaX = (this->deltaX + mouse->lLastX) / 2;
		this->deltaY = (this->deltaY + mouse->lLastY) / 2;
	}
	if (mouse->usButtonFlags > 0)
	{
		Word NANDmask = 0, ORMask = 0;
		MaxInt result = private.MBStates->BitArray[0];

		for (Byte i = 0, j = 0; i < 10, j < 5; j++, i += 2)
		{
			NANDmask |= (((1 << (i + 1))&mouse->usButtonFlags)>>(j+1));
			ORMask |= ((1 << i)&mouse->usButtonFlags)>>j;
		}
			result = result
				& 	(~NANDmask)							// NAND Mask
				| ORMask;								// OR Mask
			private.MBStates->BitArray[0] = result;
	}
}
inline void _InitializeMouse(selfptr, HWND hWnd)
{	
	private.Rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
	private.Rid.usUsage = HID_USAGE_GENERIC_MOUSE;
	private.Rid.dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
	private.Rid.hwndTarget = hWnd;
	RegisterRawInputDevices(&private.Rid, 1, sizeof(RAWINPUTDEVICE));	
}


constructMethodTable(
	.GetX = virtual(GetX),
	.GetY = virtual(GetY),
	.ButtonPressed = virtual(ButtonPressed),
	.OnMouseMoved = _TranslateMouseInput,
	.ReadMouseMovement = _ReadMouseMovement
);

Constructor(selfptr, va_list* ap)
{
	assignMethodTable(self);
	private.MouseBuffer = new(Queue, sizeof(void*), bufferSize);
	private.MBStates = new(BitField, nKeys, nKeys);
	_InitializeMouse(self, va_arg(*ap, HWND));

	return self;
}
Destructor(selfptr)
{
	ClearBuffer(private.MouseBuffer);
	delete(private.MouseBuffer);
	delete(private.MBStates);
	return self;
}
ENDCLASSDESC