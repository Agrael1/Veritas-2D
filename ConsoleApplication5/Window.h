#pragma once
#include "WinSetup.h"
#include "Exception.h"
#include "EngineCommons.h"

#define c_class WindowException

class
{
	inherits(Exception);

	privatev(
		HRESULT hr;
	);
};

#undef c_class
#define c_class ConsoleWindow

class 
{
	const void* _class;

	HANDLE hOut;	// for output
	HANDLE hIn;		// for mouse

	HANDLE hOriginalConsole;
	SMALL_RECT rWindowRect;

	HWND consoleWindow;

	Word Width;
	Word Height;

	bool bMouse;
	bool bCursor;

	methods(
		bool(*CreateConsole)(selfptr, Word width, Word height, Byte fontw, Byte fonth);
		bool(*SetCursor)(selfptr, bool value);
		bool(*Restore)(const selfptr);
		void(*OutputToScreen)(selfptr, CHAR_INFO* buffer);
		void(*BlockCursor)(selfptr, bool blocked);
		void(*SetPalette)(selfptr, COLORREF palette[16]);
	);
};

#define WND_EXCEPT_AUTO() new(WindowException,__LINE__, __FILE__, GetLastError())