#include "BitField.h"
#include "Mouse.h"
#include "Class.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

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
	return self->MBStates->method->IsSet(self->MBStates, BCode);
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
short _ReadWheelDelta(selfptr)
{
	short _proxy = self->WheelDelta;
	self->WheelDelta = 0;
	return _proxy;
}
void _TranslateMouseInput(selfptr, RAWMOUSE* mouse)
{
	// make Lerp
	account(self);
	if (mouse->usFlags == MOUSE_MOVE_RELATIVE)
	{
		this->deltaX += mouse->lLastX;
		this->deltaY += mouse->lLastY;
	}				 
	if (mouse->usButtonFlags > 0)
	{
		if (mouse->usButtonFlags == RI_MOUSE_WHEEL)
		{
			this->WheelDelta = (short)mouse->usButtonData;
			return;
		}

		Word NANDmask = 0, ORMask = 0;
		MaxInt result = self->MBStates->BitArray[0];

		for (Byte i = 0, j = 0; i < 10, j < 5; j++, i += 2)
		{
			NANDmask |= (((1 << (i + 1))&mouse->usButtonFlags)>>(j+1));
			ORMask |= ((1 << i)&mouse->usButtonFlags)>>j;
		}
			result = result
				& 	(~NANDmask)							// NAND Mask
				| ORMask;								// OR Mask
			self->MBStates->BitArray[0] = result;
	}
}
inline void InitializeMouse(selfptr, HWND hWnd)
{	
	RAWINPUTDEVICE Rid;
	Rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid.usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid.dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
	Rid.hwndTarget = hWnd;
	RegisterRawInputDevices(&Rid, 1, sizeof(RAWINPUTDEVICE));	
}


constructMethodTable(
	.GetX = virtual(GetX),
	.GetY = virtual(GetY),
	.ButtonPressed = virtual(ButtonPressed),
	.ReadWheelDelta = _ReadWheelDelta,
	.OnMouseMoved = _TranslateMouseInput,
	.ReadMouseMovement = _ReadMouseMovement
);

Constructor(selfptr, va_list* ap)
{
	assignMethodTable(self);
	self->MBStates = new(BitField, nKeys, nKeys);
	InitializeMouse(self, va_arg(*ap, HWND));

	return self;
}
Destructor(selfptr)
{
	delete(self->MBStates);
	return self;
}
ENDCLASSDESC