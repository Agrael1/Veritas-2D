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
#include <engine/win_setup.h>
#include <engine/keyboard.h>
#include "Mouse.h"

#define OPTIONAL_T WPARAM
#include <vlib/optional_t.h>

#pragma push_macro("c_class")
#undef c_class
#define c_class MessageWindow

typedef struct MessageWindow MessageWindow;

struct MessageWindow
{
	HWND ConsoleWindow;		///<Parent window
	HINSTANCE hInst;		///<Instance
	HWND Window;			///<Message window 

	// Embedded classes
	Keyboard kbd;			///<Keyboard class, translates messages to data
	Mouse mouse;			///<Mouse class, translates messages to data
	int32_t width;			///<Width of the console window	in pixels
	int32_t height;			///<Height of the console window in pixels
	bool bInFocus;			///<Is window in focus
	bool bCursorEnabled;	///<Is cursor visible
};

/// @brief Constructs a hidden messagepump window over console one
/// @param width  - Width of the console window in pixels
/// @param height - Height of the console window in pixels
/// @param ConsoleWindow - handle to parent console window
void Constructor(selfptr, int32_t width, int32_t height, HWND ConsoleWindow);
void Destructor(selfptr);

/// @brief Awakes or puts the message window to sleep
inline void ControlAwake(selfptr)
{
	PostMessage(self->Window, WM_ACTIVATE, true, 0);
}

/// @brief Make cursor stuck in some area 
inline void ConfineCursor(selfptr)
{
	RECT rekt;
	GetWindowRect(self->ConsoleWindow, &rekt);
	rekt.right = rekt.left + 1;
	ClipCursor(&rekt);
}

/// @brief Release the cursor 
inline void FreeCursor(void)
{
	ClipCursor(NULL);
}

/// @brief Make cursor disappear
inline void HideCursor(void)
{
	while (ShowCursor(FALSE) >= 0);
}

/// @brief Make cursor visible
inline void CShowCursor(void)
{
	while (ShowCursor(TRUE) < 0);
}

/// @brief release + show 
inline void EnableCursor(selfptr)
{
	self->bCursorEnabled = true;
	CShowCursor();
	FreeCursor();
}

/// @brief confine + hide 
inline void DisableCursor(selfptr)
{
	self->bCursorEnabled = false;
	HideCursor();
	ConfineCursor(self);
}

/// @brief Message handler function
LRESULT HandleMsg(selfptr, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/// @brief Message pump
/// @return on quit - returns finish code, else nullopt
inline Optional(WPARAM) ProcessMessages(void)
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (Optional(WPARAM)) { msg.wParam, true };
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return nullopt(WPARAM);
}

#ifndef CONTROL_IMPL
#pragma pop_macro("c_class")
#endif