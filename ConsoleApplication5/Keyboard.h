#pragma once
#include "Queue.h"
#include "BitField.h"
#include "EngineCommons.h"

#define c_class KeyboardEvent

class
{
	GENERATED_DESC
	privatev(
		enum virtual(Type)
		{
			Press,
			Release,
			Invalid
		}type;
		Byte code;
	);

	methods(
		bool(*IsPress)(selfptr);
		bool(*IsRelease)(selfptr);
		bool(*IsInvalid)(selfptr);
		Byte(*GetCode)(selfptr);
	);
};

#undef c_class
#define c_class Keyboard

class
{
	GENERATED_DESC
	methods(
		bool (*KeyPressed)(const struct c_class* self, Byte keycode);
		struct KeyboardEvent* (*ReadKey)(struct c_class* self);
		bool (*KeyIsEmpty)(const struct c_class* self);
		void (*ClearKey)(struct c_class* self);
	// CharRoutines
		/*Byte (*ReadChar)(struct c_class* self);
		bool (*CharIsEmpty)(const struct c_class* self);
		void (*ClearChar)(struct c_class* self);
		void (*Flush)(struct c_class* self);*/
	// Internal
		void (*OnKeyPressed)(void* self, Byte keycode);
		void (*OnKeyReleased)(void* self, Byte keycode);
		//void (*OnChar)(char character);
		void (*ClearState)(void* self);
	);

	
privatev(
	struct BitField* KeyStates;
	struct Queue* KeyBuffer;
	//struct Queue* CharBuffer;
	);
};