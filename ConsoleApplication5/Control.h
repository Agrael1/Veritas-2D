#pragma once
#include "WinSetup.h"
#include "EngineCommons.h"

#define c_class MessageWindow
class
{
	const void* _class;
	LPCWSTR wndClassName;
	HINSTANCE hInst;

	methods(
		LRESULT(*HandleMsg)(void* self, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	);
};

static unsigned int ProcessMessages()
{
	MSG msg;
	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}