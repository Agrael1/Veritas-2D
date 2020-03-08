#pragma once
#include "Window.h"
#include "VLine.h"
#include "Control.h"
#include "EngineCommons.h"

typedef struct
{
	Word width;
	Word height; 
	Byte fontw;
	Byte fonth;
}VConsoleDesc;


#define c_class VeritasEngine

class
{
	GENERATED_DESC
	const char* AppName;

	struct SwapChain* pSwap;
	struct VLine* pDevice;
	
	methods(
		// Pure Virtuals
		bool(*OnUserCreate)(void* self);
		bool(*OnUserUpdate)(void* self, double fElapsedSeconds);
		bool(*OnUserDestroy)(void* self);
		void(*HandleMouse)(void* self, struct Mouse* mouse, double fElapsedTime);
		bool(*HandleInputEvents)(void* self, const KeyboardEvent event);
		void(*HandleControls)(void* self, const struct Keyboard* kbd, double fElapsedTime);

		// Internals
		void(*CreateSwapChain)(selfptr, VConsoleDesc VPDesc);
		void(*CreateDeviceAndSwapChain)(selfptr, VConsoleDesc VPDesc);
		void(*Start)(selfptr);
		void(*Show)(selfptr);
		);

	struct ConsoleWindow Window;
	struct MessageWindow Control;
	float fElapsedSeconds;
};