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

#define T KeyboardEvent
#define N 16
#include "QueueT.h"
#undef T
#undef N
#define T char
#define N 16
#include "QueueT.h"
#undef T
#undef N

#define c_class Keyboard

class
{
	GENERATED_DESC
	methods(
		bool (*KeyPressed)(const selfptr, Byte keycode);
		Optional(KeyboardEvent) (*ReadKey)(selfptr);
		void (*ClearKey)(selfptr);
	// CharRoutines
		Optional(char)(*ReadChar)(selfptr);
		void (*ClearChar)(selfptr);
		void (*Flush)(selfptr);
	// Internal
		void (*OnKeyPressed)(selfptr, Byte keycode);
		void (*OnKeyReleased)(selfptr, Byte keycode);
		void (*OnChar)(selfptr, char character);
		void (*ClearState)(selfptr);
	);
	struct BitField* KeyStates;

	struct FixedQueue(KeyboardEvent, 16) KeyBuffer;
	struct FixedQueue(char,16) CharBuffer;
};