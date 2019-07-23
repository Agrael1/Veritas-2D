#pragma once
#include "WinSetup.h"
#include "BitField.h"
#include "EngineCommons.h"

typedef enum
{
	LEFT_MB,
	RIGHT_MB,
	MID_MB
}MButtons;

#define c_class Mouse

class
{
	GENERATED_DESC
	methods(
		int(*GetX)(const selfptr);
		int(*GetY)(const selfptr);
		void (*ReadMouseMovement)(selfptr, int* X, int* Y);
		void(*OnMouseMoved)(selfptr, RAWMOUSE* mouse);
		bool(*ButtonPressed)(selfptr, MButtons BCode);
	);

	struct BitField* MBStates;
	int deltaX, deltaY;
};