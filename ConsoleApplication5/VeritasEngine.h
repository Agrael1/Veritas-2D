#pragma once

#include "WinSetup.h"
#include "Window.h"
#include "Frame.h"
#include "Control.h"
#include "EngineCommons.h"

#define c_class VeritasEngine

class
{
	const void* _class;
	const char* AppName;
	struct ConsoleWindow* Window;
	struct Frame* Output;
	struct MessageWindow* Control;

	methods(
		// Pure Virtuals
		bool(*OnUserCreate)(void* self);
		bool(*OnUserUpdate)(void* self, double fElapsedSeconds);
		bool(*OnUserDestroy)(void* self);
		void(*HandleMouse)(void* self, struct Mouse* mouse, double fElapsedTime);
		bool(*HandleInputEvents)(void* self, const KeyboardEvent* event);
		void(*HandleControls)(void* self, const struct Keyboard* kbd, double fElapsedTime);

		// Internals
		void(*SetupScreen)(selfptr, Word width, Word height, Byte fontw, Byte fonth);
		void(*Start)(selfptr);
		void(*Show)(selfptr);
		);
};