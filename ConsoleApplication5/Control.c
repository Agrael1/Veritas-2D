#define CONTROL_IMPL
#include "Window.h"
#include "Control.h"
#include "Class.h"
#include <malloc.h>

#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)

void _CatchFocus(selfptr)
{
	DWORD numRecords;
	DWORD numRecsRecieved;
	GetNumberOfConsoleInputEvents(self->refCon->hIn, &numRecords);
	PINPUT_RECORD records = _malloca(numRecords * sizeof(INPUT_RECORD));
	ReadConsoleInput(self->refCon->hIn, records, numRecords, &numRecsRecieved);

	for (DWORD i = 0; i < numRecsRecieved; i++)
	{
		if (records[i].EventType == FOCUS_EVENT)
		{
			PostMessage(self->Window, WM_ACTIVATE, records[i].Event.FocusEvent.bSetFocus, 0);
		}
	}
	_freea(records);
}
inline void _ConfineCursor(selfptr)
{
	RECT rekt;
	GetWindowRect(self->refCon->consoleWindow, &rekt);
	rekt.right = rekt.left + 1;
	ClipCursor(&rekt);
}
inline void _FreeCursor(void)
{
	ClipCursor(NULL);
}
inline void _HideCursor(void)
{
	while (ShowCursor(FALSE) >= 0);
}
inline void _ShowCursor(void)
{
	while (ShowCursor(TRUE) < 0);
}
void _EnableCursor(selfptr)
{
	self->bCursorEnabled = true;
	_ShowCursor();
	_FreeCursor();
}
void _DisableCursor(selfptr)
{
	self->bCursorEnabled = false;
	_HideCursor();
	_ConfineCursor(self);
}

LRESULT _HandleMsg(selfptr, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
	{
		self->bInFocus = false;
		self->kbd.method->Flush(&self->kbd);
		break;
	}

#pragma region Keyboard
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000)||self->kbd.bAutorepeat)
		{
			self->kbd.method->OnKeyPressed(&self->kbd,(uint8_t)(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		self->kbd.method->OnKeyReleased(&self->kbd, (uint8_t)(wParam));
		break;
	case WM_CHAR:
		self->kbd.method->OnChar(&self->kbd, (uint8_t)(wParam));
		break;
#pragma endregion

	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// cursorless exclusive gets first dibs
		if (!self->bCursorEnabled)
		{
			if (!self->mouse.isInWindow)
			{
				SetCapture(hWnd);
				self->mouse.method->OnMouseEnter(&self->mouse);
				_HideCursor();
			}
			break;
		}

		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < self->refCon->Width && pt.y >= 0 && pt.y < self->refCon->Height)
		{
			self->mouse.method->OnMouseMove(&self->mouse, pt.x, pt.y);
			if (!self->mouse.isInWindow)
			{
				SetCapture(hWnd);
				self->mouse.method->OnMouseEnter(&self->mouse);
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				self->mouse.method->OnMouseMove(&self->mouse, pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				self->mouse.method->OnMouseLeave(&self->mouse);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		if (!self->bCursorEnabled)
		{
			_ConfineCursor(self);
			_HideCursor();
		}
		
		self->mouse.method->OnButtonPressed(&self->mouse, LEFT_MB);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		self->mouse.method->OnButtonPressed(&self->mouse, RIGHT_MB);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		self->mouse.method->OnButtonPressed(&self->mouse, MID_MB);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		self->mouse.method->OnButtonReleased(&self->mouse, LEFT_MB);
		// release mouse if outside of window
		if (!(pt.x >= 0 && pt.x < self->refCon->Width && pt.y >= 0 && pt.y < self->refCon->Height))
		{
			ReleaseCapture();
			self->mouse.method->OnMouseLeave(&self->mouse);
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		self->mouse.method->OnButtonReleased(&self->mouse, RIGHT_MB);
		// release mouse if outside of window
		if (!(pt.x >= 0 && pt.x < self->refCon->Width && pt.y >= 0 && pt.y < self->refCon->Height))
		{
			ReleaseCapture();
			self->mouse.method->OnMouseLeave(&self->mouse);
		}
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		self->mouse.method->OnButtonReleased(&self->mouse, MID_MB);
		// release mouse if outside of window
		if (!(pt.x >= 0 && pt.x < self->refCon->Width && pt.y >= 0 && pt.y < self->refCon->Height))
		{
			ReleaseCapture();
			self->mouse.method->OnMouseLeave(&self->mouse);
		}
		break;
	}
	case WM_INPUT:
	{
		if (!self->mouse.rawEnabled)
			break;

		UINT dwSize = 48;
		RAWINPUT raw;

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &dwSize,
			sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		if (raw.header.dwType == RIM_TYPEMOUSE)
		{
			self->mouse.method->OnRawMouse(&self->mouse, &raw.data.mouse);
		}
		break;
	}
	case WM_ACTIVATE:
	{
		self->bInFocus = (bool)wParam;
		if (!self->bCursorEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				_ConfineCursor(self);
				_HideCursor();
			}
			else
			{
				_FreeCursor();
				_ShowCursor();
			}
		}
		break;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve ptr to win class
	struct MessageWindow* const pWnd = (struct MessageWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward msg to class handler
	return pWnd->method->HandleMsg(pWnd, hWnd, msg, wParam, lParam);
}
LRESULT WINAPI HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		// Extract data from creation of window
		const CREATESTRUCTW* const pCreate = (CREATESTRUCTW*)(lParam);
		struct MessageWindow* const pWnd = (pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to win class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(pWnd));
		// set msgproc to to non setup handle
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)(&HandleMsgThunk));

		return pWnd->method->HandleMsg(pWnd, hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

VirtualTable{
	.HandleMsg = _HandleMsg,
	.CatchFocus = _CatchFocus,
	.EnableCursor = _EnableCursor,
	.DisableCursor = _DisableCursor
};
Constructor(selfptr, va_list *ap)
{
	InitializeVtable(self);

	self->refCon = va_arg(*ap, void*);
	self->wndClassName = L"DUMMY_CLASS";
	self->hInst = GetModuleHandle(NULL);
	self->bCursorEnabled = true;
	self->bInFocus = true;

	WNDCLASSEX wx = { 0 };
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = HandleMessageSetup;        // function which will handle messages
	wx.hInstance = self->hInst;
	wx.lpszClassName = self->wndClassName;

	if (RegisterClassEx(&wx))
	{
		WND_CALL_INFO(self->Window = CreateWindow(
			self->wndClassName,
			L"dummy_name",
			WS_CHILD, 0, 0, 0, 0,
			self->refCon->consoleWindow,
			NULL, self->hInst, self));

		SetFocus(self->Window);
	}

	RAWINPUTDEVICE Rid;
	Rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid.usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid.dwFlags = RIDEV_INPUTSINK | RIDEV_NOLEGACY;
	Rid.hwndTarget = self->Window;
	WND_CALL_INFO(RegisterRawInputDevices(&Rid, 1, sizeof(Rid)));

	// Create controls and bind mouse to the window
	construct(&self->kbd, Keyboard);
	construct(&self->mouse, Mouse);
}
Destructor(selfptr)
{
	DestroyWindow(self->Window);
	UnregisterClass(self->wndClassName, self->hInst);
}
ENDCLASSDESC

extern inline Optional(WPARAM) ProcessMessages();