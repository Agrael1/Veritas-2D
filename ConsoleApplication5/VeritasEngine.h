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
		bool(*SetupScreen)(void* self, Word width, Word height, Byte fontw, Byte fonth);
		void(*Start)(void* _self);
	);
};