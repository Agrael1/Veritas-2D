#pragma once
#include "WinSetup.h"
#include "Exception.h"
#include "EngineCommons.h"

#define c_class WindowException

class
{
	inherits(Exception);
	HRESULT hr;
};

#undef c_class
#define c_class ConsoleWindow

class 
{
	GENERATED_DESC
	methods(
		COORD(*CreateConsole)(selfptr, Word width, Word height, Byte fontw, Byte fonth);
		bool(*SetCursor)(selfptr, bool value);
		bool(*Restore)(const selfptr);
		void(*OutputToScreen)(selfptr);
		void(*SetPalette)(selfptr, COLORREF palette[16]);
	);

	HANDLE hOut;	
	HANDLE hIn;	

	HANDLE hOriginalConsole;
	HWND consoleWindow;
	CHAR_INFO** ppBuffer;

	SMALL_RECT rWindowRect;

	long lOriginalParams;

	Word Width;
	Word Height;
};

#define WND_EXCEPT_AUTO() new(WindowException,__LINE__, __FILE__, GetLastError())
#define WND_CALL_INFO(call) if(!(call)) throw(WND_EXCEPT_AUTO())