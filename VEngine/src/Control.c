#define CONTROL_IMPL
#include "Control.h"
#include <malloc.h>

#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#define WND_CLASSNAME TEXT("DUMMY_CLASS")

#define WND_CALL_INFO(x) x

extern inline void ConfineCursor(selfptr);
extern inline void FreeCursor(void);
extern inline void HideCursor(void);
extern inline void CShowCursor(void);
extern inline void EnableCursor(selfptr);
extern inline void DisableCursor(selfptr);
extern inline void ControlAwake(selfptr);

LRESULT HandleMsg(selfptr, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
	{
		self->bInFocus = false;
		KeyboardFlush(&self->kbd);
		ClearState(&self->kbd);
		break;
	}

#pragma region Keyboard
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || self->kbd.bAutorepeat)
		{
			OnKeyPressed(&self->kbd, (uint8_t)(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		OnKeyReleased(&self->kbd, (uint8_t)(wParam));
		break;
	case WM_CHAR:
		OnChar(&self->kbd, (uint8_t)(wParam));
		break;
#pragma endregion

	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// cursorless exclusive gets first dibs
		if (!self->bCursorEnabled)
		{
			if (!self->mouse.bIsInWindow)
			{
				SetCapture(hWnd);
				OnMouseEnter(&self->mouse);
				HideCursor();
			}
			break;
		}

		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < self->width && pt.y >= 0 && pt.y < self->height)
		{
			OnMouseMove(&self->mouse, pt.x, pt.y);
			if (!self->mouse.bIsInWindow)
			{
				SetCapture(hWnd);
				OnMouseEnter(&self->mouse);
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				OnMouseMove(&self->mouse, pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				OnMouseLeave(&self->mouse);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		if (!self->bCursorEnabled)
		{
			ConfineCursor(self);
			HideCursor();
		}

		OnButtonPressed(&self->mouse, LEFT_MB);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		OnButtonPressed(&self->mouse, RIGHT_MB);
		break;
	}
	case WM_MBUTTONDOWN:
	{
		OnButtonPressed(&self->mouse, MID_MB);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		OnButtonReleased(&self->mouse, LEFT_MB);
		// release mouse if outside of window
		if (!(pt.x >= 0 && pt.x < self->width && pt.y >= 0 && pt.y < self->height))
		{
			ReleaseCapture();
			OnMouseLeave(&self->mouse);
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		OnButtonReleased(&self->mouse, RIGHT_MB);
		// release mouse if outside of window
		if (!(pt.x >= 0 && pt.x < self->width && pt.y >= 0 && pt.y < self->height))
		{
			ReleaseCapture();
			OnMouseLeave(&self->mouse);
		}
		break;
	}
	case WM_MBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		OnButtonReleased(&self->mouse, MID_MB);
		// release mouse if outside of window
		if (!(pt.x >= 0 && pt.x < self->width && pt.y >= 0 && pt.y < self->height))
		{
			ReleaseCapture();
			OnMouseLeave(&self->mouse);
		}
		break;
	}
	case WM_INPUT:
	{
		if (!self->mouse.bRawEnabled)
			break;

		UINT dwSize = 48;

		if (GetRawInputData(
			(HRAWINPUT)(lParam),
			RID_INPUT,
			NULL,
			&dwSize,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			// bail msg processing if error
			break;
		}

		RAWINPUT raw;

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &dwSize,
			sizeof(RAWINPUTHEADER)) != dwSize)
			OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

		if (raw.header.dwType == RIM_TYPEMOUSE)
		{
			OnRawMouse(&self->mouse, raw.data.mouse.lLastX, raw.data.mouse.lLastY);
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
				ConfineCursor(self);
				HideCursor();
			}
			else
			{
				FreeCursor();
				CShowCursor();
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
	MessageWindow* const pWnd = (MessageWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward msg to class handler
	return HandleMsg(pWnd, hWnd, msg, wParam, lParam);
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

		return HandleMsg(pWnd, hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void Constructor(selfptr, int32_t width, int32_t height, HWND ConsoleWindow)
{
	self->ConsoleWindow = ConsoleWindow;
	self->hInst = GetModuleHandle(NULL);
	self->bCursorEnabled = true;
	self->bInFocus = true;
	self->width = width;
	self->height = height;

	WNDCLASSEX wx = { 0 };
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = HandleMessageSetup;        // function which will handle messages
	wx.hInstance = self->hInst;
	wx.lpszClassName = WND_CLASSNAME;

	if (RegisterClassEx(&wx))
	{
		WND_CALL_INFO(self->Window = CreateWindow(
			WND_CLASSNAME,
			TEXT("dummy_name"),
			WS_CHILD, 0, 0, 0, 0,
			ConsoleWindow,
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
	Keyboard_ctor(&self->kbd);
	Mouse_ctor(&self->mouse);
}
void Destructor(selfptr)
{
	DestroyWindow(self->Window);
	UnregisterClass(WND_CLASSNAME, self->hInst);

	Mouse_dtor(&self->mouse);
	Keyboard_dtor(&self->kbd);
}

extern inline Optional(WPARAM) ProcessMessages();

#define OPTIONAL_RESIDE
#define OPTIONAL_T WPARAM
#include <VOptionalT.c>