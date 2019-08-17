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
	GENERATED_DESC

	HANDLE hOut;	
	HANDLE hIn;	

	HANDLE hOriginalConsole;
	SMALL_RECT rWindowRect;

	HWND consoleWindow;

	Word Width;
	Word Height;

	methods(
		COORD(*CreateConsole)(selfptr, Word width, Word height, Byte fontw, Byte fonth);
		bool(*SetCursor)(selfptr, bool value);
		bool(*Restore)(const selfptr);
		void(*OutputToScreen)(selfptr, CHAR_INFO* buffer);
		void(*SetPalette)(selfptr, COLORREF palette[16]);
		void(*BlockCursor)(selfptr);
		void(*ReleaseCursor)(selfptr);
	);
};

#define WND_EXCEPT_AUTO() new(WindowException,__LINE__, __FILE__, GetLastError())
#define WND_CALL_INFO(call) if(!(call)) throw(WND_EXCEPT_AUTO())