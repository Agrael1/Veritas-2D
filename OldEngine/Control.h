#pragma once
#include "Templates.h"
#include "Keyboard.h"
#include "Mouse.h"

#define T WPARAM
#include "OptionalT.h"
#undef T

#define c_class MessageWindow
class
{
	struct ConsoleWindow* refCon;
	LPCWSTR wndClassName;
	HINSTANCE hInst;
	HWND Window;

	// Embedded classes
	struct Keyboard kbd;
	struct Mouse mouse;
	bool bInFocus;
	bool bCursorEnabled;
};
interface
{
	LRESULT(*HandleMsg)(void* self, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void(*CatchFocus)(selfptr);
	void (*EnableCursor)(selfptr);
	void (*DisableCursor)(selfptr);
};
ComposeType;

inline Optional(WPARAM) ProcessMessages()
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
#undef c_class
#endif