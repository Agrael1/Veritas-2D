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
bool virtual(IsPress)(struct c_class* self)
{
	account(self);
	return private.type == MPress;
}
bool virtual(IsRelease)(struct c_class* self)
{
	account(self);
	return private.type == MRelease;
}
bool virtual(IsInvalid)(struct c_class* self)
{
	account(self);
	return private.type == MInvalid;
}
Byte virtual(GetCode)(void* self)
{
	account(self);
	return private.code;
}

Constructor(void* self, va_list* ap)
{
	account(self);

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
#define c_class Mouse
void _ReadMouseMovement(void* self, int* X, int* Y)
{
	account(self);
	if (X)
		*X = this->deltaX;
	if (Y)
		*Y = this->deltaY;
	
	this->deltaX = 0; this->deltaY = 0;
}
void _OnMouseMoved(void* self, RAWMOUSE* mouse)
{
	// make Lerp
	account(self);
	if (mouse->usFlags == 0)
	{
		this->deltaX = (this->deltaX + mouse->lLastX) / 2;
		this->deltaY = (this->deltaY + mouse->lLastY) / 2;
	}
}
void _InitializeMouse(void* self, HWND hWnd)
{
	account(self);
	
	private.Rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
	private.Rid.usUsage = HID_USAGE_GENERIC_MOUSE;
	private.Rid.dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
	private.Rid.hwndTarget = hWnd;
	RegisterRawInputDevices(&private.Rid, 1, sizeof(RAWINPUTDEVICE));	
}

constructMethodTable(
	.InitializeMouse = _InitializeMouse,
	.OnMouseMoved = _OnMouseMoved,
	.ReadMouseMovement = _ReadMouseMovement
);

Constructor(void* self, va_list* ap)
{
	account(self);
	assignMethodTable(this);
	return this;
}
Destructor(void* self)
{
	return self;
}
ENDCLASSDESC