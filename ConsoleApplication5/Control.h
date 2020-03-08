#pragma once
#include "Templates.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "EngineCommons.h"

#define T WPARAM
#include "OptionalT.h"
#undef T

#define c_class MessageWindow
class
{
	GENERATED_DESC
	methods(
		LRESULT(*HandleMsg)(void* self, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void(*CatchFocus)(selfptr);
		void (*EnableCursor)(selfptr);
		void (*DisableCursor)(selfptr);
	);

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

inline Optional(WPARAM) ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (Optional(WPARAM)) { msg.wParam, true };
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return nullopt(WPARAM)();
}