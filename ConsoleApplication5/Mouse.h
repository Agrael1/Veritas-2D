#pragma once
#include "WinSetup.h"
#include "EngineCommons.h"

#define c_class MouseEvent
class 
{
	const void* _class;
	privatev(

	enum virtual(Type)
	{
			MPress,
			MRelease,
			MInvalid
	}type;
	Byte code;
	);

	methods(
	bool(*IsPress)(struct c_class* self);
	bool(*IsRelease)(struct c_class* self);
	bool(*IsInvalid)(struct c_class* self);
	Byte(*GetCode)(struct c_class* self);
	);
};

#undef c_class
#define c_class Mouse

class
{
	const void* _class;
	privatev(
		RAWINPUTDEVICE Rid;
	);
	methods(
		void(*InitializeMouse)(void* self, HWND hWnd);
		void(*OnMouseMoved)(void* self, RAWMOUSE* mouse);
		void (*ReadMouseMovement)(void* self, int* X, int* Y);
	);

	int deltaX, deltaY;
};