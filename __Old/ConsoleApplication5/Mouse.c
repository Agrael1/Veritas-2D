#define MOUSE_IMPL
#include "Mouse.h"


bool virtual(ButtonPressed)(selfptr, MButtons BCode)
{
	switch (BCode)
	{
	case LEFT_MB:
		return self->LeftIsPressed;
	case RIGHT_MB:
		return self->RightIsPressed;
	case MID_MB:
		return self->MidIsPressed;
	default:
		return false;
	}
}
void _ReadMouseMovement(selfptr, short* X, short* Y)
{
	if (X)
		*X = self->deltaX;
	if (Y)
		*Y = self->deltaY;
	self->deltaX = 0; self->deltaY = 0;
}
short _ReadWheelDelta(selfptr)
{
	short _proxy = self->WheelDelta;
	self->WheelDelta = 0;
	return _proxy;
}
void _OnRawMouse(selfptr, RAWMOUSE* mouse)
{
	// make Lerp
	if (mouse->usFlags == MOUSE_MOVE_RELATIVE)
	{
		self->deltaX += (short)mouse->lLastX;
		self->deltaY += (short)mouse->lLastY;
	}				 
	if (mouse->usButtonFlags > 0)
	{
		if (mouse->usButtonFlags == RI_MOUSE_WHEEL)
		{
			self->WheelDelta +=(short)mouse->usButtonData;
			return;
		}
	}
}
void _OnMouseMove(selfptr, int newx, int newy)
{
	self->posX = newx;
	self->posY = newy;
}
void _OnMouseLeave(selfptr)
{
	self->isInWindow = false;
	self->MouseEvents.method->push(&self->MouseEvents, Leave);
}
void _OnMouseEnter(selfptr)
{
	self->isInWindow = true;
	self->MouseEvents.method->push(&self->MouseEvents, Leave);
}
void _OnButtonPressed(selfptr, MButtons BCode) 
{
	switch (BCode)
	{
	case LEFT_MB:
		self->LeftIsPressed = true;
		self->MouseEvents.method->push(&self->MouseEvents, LPress);
		return;
	case RIGHT_MB:
		self->RightIsPressed = true;
		self->MouseEvents.method->push(&self->MouseEvents, RPress);
		return;
	case MID_MB:
		self->MidIsPressed = true;
		self->MouseEvents.method->push(&self->MouseEvents, MPress);
		return;
	default:
		return;
	}
}
void _OnButtonReleased(selfptr, MButtons BCode)
{
	switch (BCode)
	{
	case LEFT_MB:
		self->LeftIsPressed = false;
		self->MouseEvents.method->push(&self->MouseEvents, LRelease);
		return;
	case RIGHT_MB:
		self->RightIsPressed = false;
		self->MouseEvents.method->push(&self->MouseEvents, RRelease);
		return;
	case MID_MB:
		self->MidIsPressed = false;
		self->MouseEvents.method->push(&self->MouseEvents, MRelease);
		return;
	default:
		return;
	}
}

Optional(char) _ReadMouseEvents(selfptr)
{
	return self->MouseEvents.method->pop(&self->MouseEvents);
}
void virtual(Flush)(selfptr)
{
	self->MouseEvents.method->wipe(&self->MouseEvents);
}

VirtualTable{
	.OnMouseMove = _OnMouseMove,
	.OnMouseLeave = _OnMouseLeave,
	.OnMouseEnter = _OnMouseEnter,
	.OnRawMouse = _OnRawMouse,
	.OnButtonPressed = _OnButtonPressed,
	.OnButtonReleased = _OnButtonReleased,

	.ReadMouseMovement = _ReadMouseMovement,
	.ReadWheelDelta = _ReadWheelDelta,
	.ReadMouseEvents = _ReadMouseEvents,
	.Flush = virtual(Flush),
	.ButtonPressed = virtual(ButtonPressed)	
};
Constructor(selfptr, va_list* ap)
{
	InitializeVtableEx(self);
	construct(&self->MouseEvents, RingT(char, 16));
	self->posX = self->posY = 0;
	self->deltaX = self->deltaY = 0;
	self->WheelDelta = 0;
	self->isInWindow = false;
	self->LeftIsPressed = false;
	self->RightIsPressed = false;
	self->MidIsPressed = false;
	self->rawEnabled = false;
}
ENDCLASSDESCDD