#pragma once
#include "Queue.h"
#include "EventQueue.h"
#include "BitField.h"
#include "EngineCommons.h"

#define c_class KeyboardEvent

class
{
	GENERATED_DESC
	methods(
		bool(*IsPress)(const selfptr);
		bool(*IsRelease)(const selfptr);
		bool(*IsInvalid)(const selfptr);
		Byte(*GetCode)(const selfptr);
	);

	privatev(
		enum virtual(Type)
		{
			Press,
			Release,
			Invalid
		}type;
		Byte code;
	);
};

#undef c_class
#define c_class Keyboard

class
{
	GENERATED_DESC
	methods(
		bool (*KeyPressed)(const selfptr, Byte keycode);
		struct KeyboardEvent* (*ReadKey)(selfptr);
		void (*ClearKey)(selfptr);
	// CharRoutines
		char (*ReadChar)(selfptr);
		void (*ClearChar)(selfptr);
		void (*Flush)(selfptr);
	// Internal
		void (*OnKeyPressed)(selfptr, Byte keycode);
		void (*OnKeyReleased)(selfptr, Byte keycode);
		void (*OnChar)(selfptr, char character);
		void (*ClearState)(selfptr);
	);
	struct BitField* KeyStates;

	struct EventQueue KeyBuffer;
	struct EventQueue CharBuffer;
};