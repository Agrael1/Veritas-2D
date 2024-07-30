#pragma once
#include "WinSetup.h"
#include "Exception.h"
#include <stdint.h>

#define c_class WindowException

class
{
	inherit(Exception);
	HRESULT hr;
};
interface
{
	implements(Exception);
	String(*GetErrorString)(selfptr);
};
ComposeType;

#undef c_class
#define c_class ConsoleWindow

class 
{
	HANDLE hOut;	
	HANDLE hIn;	

	HANDLE hOriginalConsole;
	HWND consoleWindow;
	CHAR_INFO** ppBuffer;

	SMALL_RECT rWindowRect;

	long lOriginalParams;

	uint16_t Width;
	uint16_t Height;
};
interface
{
	COORD(*CreateConsole)(selfptr, uint16_t width, uint16_t height, uint8_t fontw, uint8_t fonth);
	bool(*SetCursor)(selfptr, bool value);
	bool(*Restore)(const selfptr);
	void(*OutputToScreen)(selfptr);
	void(*SetPalette)(selfptr, COLORREF palette[16]);
};
ComposeType;

#define WND_EXCEPT_AUTO() new(WindowException,__LINE__, __FILE__, GetLastError())
#define WND_CALL_INFO(call) if(!(call)) throw(WND_EXCEPT_AUTO())
#ifndef WINDOW_IMPL
#undef c_class
#endif