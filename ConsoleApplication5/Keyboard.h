#pragma once
#include "Queue.h"
#include "BitField.h"
#include "EngineCommons.h"


#define c_class KeyboardEvent

class
{
	const void* _class;
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
		bool(*IsPress)(struct c_class* self);
		bool(*IsRelease)(struct c_class* self);
		bool(*IsInvalid)(struct c_class* self);
		Byte(*GetCode)(struct c_class* self);
	);
};

#undef c_class
#define c_class Keyboard

class
{
	const void* _class;
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