#pragma once
#include "WinSetup.h"
#include "Queue.h"
#include "BitField.h"
#include "EngineCommons.h"

typedef enum
{
	LEFT_MB,
	RIGHT_MB,
	MID_MB
}MButtons;

#define c_class MouseEvent
class 
{
	const void* _class;
	privatev(

	enum virtual(Type)
	{
			MPress,
			MRelease,
			MWUp,
			MWDown,
			MInvalid
	}type;
	Word code;
	);

	methods(
		enum virtual(Type) (*GetType)(const selfptr);
		bool(*IsInvalid)(const selfptr);
	);
};

#undef c_class
#define c_class Mouse

class
{
	const void* _class;
	methods(
		int(*GetX)(const selfptr);
		int(*GetY)(const selfptr);
		void (*ReadMouseMovement)(selfptr, int* X, int* Y);
		void(*InitializeMouse)(selfptr, HWND hWnd);
		void(*OnMouseMoved)(selfptr, RAWMOUSE* mouse);
		bool(*ButtonPressed)(selfptr, MButtons BCode);
	);

	privatev(
		RAWINPUTDEVICE Rid;
		struct Queue* MouseBuffer;
		struct BitField* MBStates;
	);
	
	int deltaX, deltaY;
};

