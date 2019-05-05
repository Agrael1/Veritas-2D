#pragma once
#include "WinSetup.h"
#include "Queue.h"
#include "EngineCommons.h"

#define c_class MouseEvent
class 
{
	const void* _class;
	privatev(

	enum virtual(Type)
	{
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			MWUp,
			MWDown,
			MInvalid
	}type;
	bool leftIsPressed;
	bool rightIsPressed;
	bool midIsPressed;
	int x;
	int y;
	);

	methods(
		enum virtual(Type) (*GetType)(const selfptr);
		bool(*IsInvalid)(const selfptr);
		int(*GetX)(const selfptr);
		int(*GetY)(const selfptr);
		bool(*RightIsPressed)(const selfptr);
		bool(*LeftIsPressed)(const selfptr);
		bool(*MidIsPressed)(const selfptr);
	);
};

#undef c_class
#define c_class Mouse

class
{
	const void* _class;
	methods(
		void(*InitializeMouse)(selfptr, HWND hWnd);
		void(*OnMouseMoved)(selfptr, RAWMOUSE* mouse);
		void (*ReadMouseMovement)(selfptr, int* X, int* Y);
	);

	privatev(
		RAWINPUTDEVICE Rid;
		struct Queue* MouseBuffer;
	);
	
	int deltaX, deltaY;
};