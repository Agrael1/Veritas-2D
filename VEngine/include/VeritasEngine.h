// Copyright (c) 2020
 //      Ilya Doroshenko, xdoros01
 //      agrael@live.ru, xdoros01@fit.vutbr.cz
 // All rights reserved.
 //
 // This code is licensed under the MIT License.
 //
 // Permission is hereby granted, free of charge, to any person obtaining a copy
 // of this software and associated documentation files(the "Software"), to deal
 // in the Software without restriction, including without limitation the rights
 // to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 // copies of the Software, and to permit persons to whom the Software is
 // furnished to do so, subject to the following conditions :
 //
 // The above copyright notice and this permission notice shall be included in
 // all copies or substantial portions of the Software.
 //
 // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 // THE SOFTWARE.

#pragma once
#include "Window.h"
#include "Control.h"
#include "SwapChain.h"

typedef struct
{
	uint16_t width;
	uint16_t height;
	uint8_t fontw;
	uint8_t fonth;
}VConsoleDesc;

#pragma push_macro("c_class")
#undef c_class
#define c_class VeritasEngine

typedef struct VeritasEngine VeritasEngine;
typedef const struct IVeritasEngine IVeritasEngine;

struct VeritasEngine
{
	IVeritasEngine* method;
	//struct VLine* pDevice;

	SwapChain Swap;
	ConsoleWindow Window;
	MessageWindow Control;
	float fElapsedSeconds;
};
struct IVeritasEngine
{
	// Pure Virtuals
	bool(*OnUserCreate)(void* self);
	bool(*OnUserUpdate)(void* self, double fElapsedSeconds);
	bool(*OnUserDestroy)(void* self);
	void(*HandleMouse)(void* self, struct Mouse* mouse, double fElapsedTime);
	bool(*HandleInputEvents)(void* self, const KeyboardEvent event);
	void(*HandleControls)(void* self, const struct Keyboard* kbd, double fElapsedTime);
};

void Constructor(selfptr, VConsoleDesc screenparams);
void Destructor(selfptr);


void Start(selfptr);
inline void Show(selfptr)
{
	OutputToScreen(&self->Window, PresentFrame(&self->Swap));
}

// Internals
//void(*CreateSwapChain)(selfptr, VConsoleDesc VPDesc);
//void(*CreateDeviceAndSwapChain)(selfptr, VConsoleDesc VPDesc);


#ifndef VENGINE_IMPL
#pragma pop_macro("c_class")
#endif