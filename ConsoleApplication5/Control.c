#include "Window.h"
#include "Control.h"
#include "Class.h"
#include <malloc.h>


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
void _BlockCursor(selfptr)
{
	RECT rekt;
	GetWindowRect(self->refCon->consoleWindow, &rekt);
	rekt.right = rekt.left + 1;
	ClipCursor(&rekt);
	while (ShowCursor(false)>=0);
	self->bCursorEnabled = false;
}
void _ReleaseCursor(selfptr)
{
	while (ShowCursor(true)<0);
	ClipCursor(NULL);
	self->bCursorEnabled = true;
}

LRESULT _HandleMsg(selfptr, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	account(self);
	switch (msg)
	{
	case WM_KILLFOCUS:
	{
		self->bInFocus = false;
		_ReleaseCursor(self);
		break;
	}
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
	case WM_ACTIVATE:
	{
		self->bInFocus = (bool)wParam;
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
inline void _CreateControl(selfptr)
{
	WNDCLASSEX wx = {0};
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = HandleMessageSetup;        // function which will handle messages
	wx.hInstance = self->hInst;
	wx.lpszClassName = self->wndClassName;

	if (RegisterClassEx(&wx)) 
	{
		self->Window = CreateWindow(
			self->wndClassName,
			L"dummy_name",
			WS_CHILD, 0, 0, 0, 0,
			self->refCon->consoleWindow,
			NULL, self->hInst, self);
		
		SetFocus(self->Window);
	}
}

constructMethodTable(
	.HandleMsg = _HandleMsg,
	.CatchFocus = _CatchFocus,
	.BlockCursor = _BlockCursor,
	.ReleaseCursor = _ReleaseCursor
);

Constructor(selfptr, va_list *ap)
{
	assignMethodTable(self);

	self->refCon = va_arg(*ap, void*);
	self->wndClassName = L"DUMMY_CLASS";
	self->hInst = GetModuleHandle(NULL);
	self->bCursorEnabled = true;
	self->bInFocus = true;

	_CreateControl(self);

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