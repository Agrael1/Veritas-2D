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
#include "WinSetup.h"
#include <RuntimeClass.h>
#include <stdbool.h>

#pragma push_macro("c_class")
#undef c_class
#define c_class ConsoleWindow

typedef struct ConsoleWindow ConsoleWindow;

struct ConsoleWindow
{
	HANDLE hOut;			///<Handle to standard output buffer
	HANDLE hIn;				///<Handle to standard input buffer
	HANDLE hOriginalConsole;///<Handle to old output buffer

	HWND ConsoleWindow;		///<Handle to standard output buffer

	SMALL_RECT rWindowRect;	///<Write rectangle
	COORD Dimensions;		///<Dimensions of the output buffer in symbols
	long lOriginalParams;	///<Old console parameters
};

/// @brief Restores console to its previous state  
inline void Restore(selfptr);

inline void Constructor(selfptr)
{
	self->ConsoleWindow = GetConsoleWindow();
	self->hIn = GetStdHandle(STD_INPUT_HANDLE);
	self->hOut = self->hOriginalConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	self->Dimensions = (COORD){ 0, 0 };
	self->lOriginalParams = 0;
	self->rWindowRect = (SMALL_RECT){ 0,0,0,0 };
	self->ConsoleWindow = 0;
}
inline void Destructor(selfptr)
{
	Restore(self);
}

/// @brief Creates buffer and expands console window, removes minimize button, sets projection pixel size
/// @param width - width of the buffer in symbols
/// @param height - height of the buffer in symbols
/// @param fontw - width of the symbol (pixel)
/// @param fonth - height of the symbol (pixel)
/// @return - dimensions of the output buffer
COORD CreateConsole(selfptr, uint16_t width, uint16_t height, uint8_t fontw, uint8_t fonth);

/// @brief Catches focus events for transfer into control window
/// @return true if focus was caught, false - else
bool CatchFocus(selfptr);

/// @brief Hides or shows console cursor
/// @param show - if false - cursor hidden
/// @return true if operation succeeds
inline bool ConShowCursor(selfptr, bool show)
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = show;
	return (bool)SetConsoleCursorInfo(self->hOut, &info);
} 
inline void Restore(selfptr)
{
	SetConsoleActiveScreenBuffer(self->hOriginalConsole);
	if (self->hOut != self->hOriginalConsole) {
		CloseHandle(self->hOut);
		self->hOut = self->hOriginalConsole;
	}
	SetWindowLongPtr(self->ConsoleWindow, GWL_STYLE, self->lOriginalParams);
}

/// @brief Sets a new color scheme to the console
/// @param palette - 16 colors
inline void SetPalette(selfptr, COLORREF palette[16])
{
	CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
	csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(self->hOut, &csbiex);
	memcpy(csbiex.ColorTable, palette, 16 * sizeof(COLORREF));
	SetConsoleScreenBufferInfoEx(self->hOut, &csbiex);
}

/// @brief Outputs a frame to the console   
/// @param buffer - pointer to a frame buffer
inline void OutputToScreen(selfptr, CHAR_INFO* buffer)
{
	WriteConsoleOutput(
		self->hOut,
		buffer,
		self->Dimensions,
		(COORD) {
		0, 0
	},
		& self->rWindowRect);
}

#define WND_CALL_INFO(call) (call) //TODO

#ifndef WINDOW_IMPL
#pragma pop_macro("c_class")
#endif