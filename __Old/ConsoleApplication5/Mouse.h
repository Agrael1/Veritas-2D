#pragma once
#include "WinSetup.h"
#include "Templates.h"

typedef enum 
{
	LEFT_MB,
	RIGHT_MB,
	MID_MB
}MButtons;

typedef enum
{
	LPress,
	LRelease,
	RPress,
	RRelease,
	MPress,
	MRelease,
	WheelUp,
	WheelDown,
	Move,
	Enter,
	Leave,
}MouseEvent;

#define c_class Mouse

class
{
	Ring(char, 16) MouseEvents;

	short posX, posY;
	short deltaX, deltaY;
	short WheelDelta;
	bool isInWindow;
	bool LeftIsPressed;
	bool RightIsPressed;
	bool MidIsPressed;
	bool rawEnabled;
};
interface
{
	void (*OnMouseMove)(selfptr, int newx, int newy);
	void (*OnMouseLeave)(selfptr);
	void (*OnMouseEnter)(selfptr);
	void (*OnRawMouse)(selfptr, RAWMOUSE* mouse);
	void (*OnButtonPressed)(selfptr, MButtons BCode);
	void (*OnButtonReleased)(selfptr, MButtons BCode);

	void (*ReadMouseMovement)(selfptr, short* X, short* Y);
	short(*ReadWheelDelta)(selfptr);
	Optional(char) (*ReadMouseEvents)(selfptr);
	void (*Flush)(selfptr);
	bool(*ButtonPressed)(selfptr, MButtons BCode);
};
ComposeTypeEx;

#ifndef MOUSE_IMPL
#undef c_class
#endif
