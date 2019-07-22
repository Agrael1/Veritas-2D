#include "Control.h"
#include "Class.h"

LRESULT _HandleMsg(selfptr, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	account(self);
	switch (msg)
	{
	case WM_KILLFOCUS:
		SetFocus(hWnd);
		break;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000))
		{
			this->kbd.method->OnKeyPressed(&this->kbd,(Byte)(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		this->kbd.method->OnKeyReleased(&this->kbd, (Byte)(wParam));
		break;
	case WM_INPUT:
		{
			UINT dwSize = 48;
			RAWINPUT raw;

			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &dwSize,
				sizeof(RAWINPUTHEADER)) != dwSize)
				OutputDebugString(TEXT("GetRawInputData does not return correct size !\n"));

			if (raw.header.dwType == RIM_TYPEMOUSE)
			{
				this->mouse.method->OnMouseMoved(&this->mouse, &raw.data.mouse);
			}

			break;
		}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve ptr to win class
	struct MessageWindow* const pWnd = (struct MessageWindow*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward msg to class handler
	return pWnd->method->HandleMsg(pWnd, hWnd, msg, wParam, lParam);
}
LRESULT HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
inline void _CreateControl(selfptr, HWND parent)
{
	WNDCLASSEX wx = {0};
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = HandleMessageSetup;        // function which will handle messages
	wx.hInstance = self->hInst;
	wx.lpszClassName = self->wndClassName;

	if (RegisterClassEx(&wx)) 
	{
		self->Window = CreateWindowExW(0,
			self->wndClassName,
			L"dummy_name",
			WS_CHILD, 0, 0, 0, 0,
			parent,
			NULL, self->hInst, self);
		
		SetFocus(self->Window);
	}
}

constructMethodTable(
	.HandleMsg = _HandleMsg
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);

	self->wndClassName = L"DUMMY_CLASS";
	self->hInst = GetModuleHandle(NULL);

	_CreateControl(self, va_arg(*ap, HWND));

	// Create controls and bind mouse to the window
	construct(&self->kbd, Keyboard);
	construct(&self->mouse, Mouse, self->Window);
	return self;
}
Destructor(selfptr)
{
	UnregisterClass(self->wndClassName, self->hInst);
	deconstruct(&self->kbd);
	deconstruct(&self->mouse);
	return self;
}
ENDCLASSDESC