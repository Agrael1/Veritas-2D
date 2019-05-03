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
		bool(*OnUserCreate)(void* self);
		bool(*OnUserUpdate)(void* self);
		void(*HandleMouse)(void* self, struct Mouse* mouse, const double fElapsedTime);
		bool(*HandleInputEvents)(void* self, struct KeyboardEvent event);
		void(*HandleControls)(void* self, const struct Keyboard* kbd, const double fElapsedTime);
		bool(*SetupScreen)(void* self, Word width, Word height, Byte fontw, Byte fonth);
		void(*Start)(void* _self);
		void(*Show)(void* self);
		);
};