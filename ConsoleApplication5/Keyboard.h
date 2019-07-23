#pragma once
#include "EventQueue.h"
#include "BitField.h"
#include "EngineCommons.h"

typedef struct 
{
	enum Type
	{
		Press,
		Release,
		Invalid
	}type;
	Byte code;
}KeyboardEvent;

#define c_class Keyboard

class
{
	GENERATED_DESC
	methods(
		bool (*KeyPressed)(const selfptr, Byte keycode);
		KeyboardEvent* (*ReadKey)(selfptr);
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