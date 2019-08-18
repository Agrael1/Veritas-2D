#include "Class.h"
#include <stdlib.h>
#include "StringStream.h"
#include "VeritasEngine.h"

__declspec(thread) char buf[10];
volatile bool bActive = false;

bool virtual(HandleInputEvents)(void* self, const KeyboardEvent* event)
{
	if (event->type == Press)
	{
		// check if the event was for the escape key
		if (event->code == VK_ESCAPE)
		{
			return false;
		}
	}
	return true;
}
bool _PassEvents(selfptr)
{
	KeyboardEvent *event;
	while ((event = self->Control->kbd.method->ReadKey(&self->Control->kbd))!=nullptr)
	{
		return self->method->HandleInputEvents(self, event);
	}
	return true;
}

void _Show(selfptr)
{
	self->Window->method->OutputToScreen(self->Window, self->Output->localFrame);
}
void _SetupScreen(selfptr, Word width, Word height, Byte fontw, Byte fonth)
{
	account(self);

	// default setup for fast access
	COORD frame = this->Window->method->CreateConsole(this->Window, width, height, fontw, fonth);

	this->Output = new(Frame, frame.X, frame.Y);
	_Show(this);
}
DWORD _stdcall _GameThread(selfptr)
{
	account(self);
	this->Control = new(MessageWindow, this->Window);
	int gResult;

	if (!this->method->OnUserCreate(this))
		return 2;

	// Time counting
	LARGE_INTEGER StartingTime, EndingTime;
	LARGE_INTEGER Frequency;
	double fElapsedSeconds;
	int rtime = 0;
	int middle = 0, k = 0;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	// Game Loop
	while (bActive)
	{
		QueryPerformanceCounter(&EndingTime);
		fElapsedSeconds = (double)(EndingTime.QuadPart - StartingTime.QuadPart) / (double)Frequency.QuadPart;
		StartingTime = EndingTime;

		// Catch a focus if not in it
		if (!self->Control->bInFocus)
			self->Control->method->CatchFocus(self->Control);

		// Read Messages
		if (gResult = ProcessMessages() != 0)
			return gResult;

		// Process queue
		if (!_PassEvents(this))
			return 1;

		// Process mouse
		if (this->method->HandleMouse)
			this->method->HandleMouse(this, &this->Control->mouse, fElapsedSeconds);

		// Process continuous input
		if (this->method->HandleControls)
			this->method->HandleControls(this, &this->Control->kbd, fElapsedSeconds);

		// render frame
		if (!this->method->OnUserUpdate(this, fElapsedSeconds))
			return 0;

		_Show(this);

		if (k++ == 60)
		{
			rtime = (int)(1.0 / fElapsedSeconds);
			middle = (rtime + middle) / 2;
			k = 0;
			SetConsoleTitleA(_itoa(middle, buf, 10));
		}
	}

	if (this->method->OnUserDestroy&&!this->method->OnUserDestroy(this))
	{
		return 3;
	}

	return 0;
}
void _Start(selfptr)
{
	DWORD dwThreadID;
	HANDLE hThread;
	DWORD ExitCode;

	bActive = true;		
	// start a game thread
	hThread = CreateThread(
		NULL,
		0,
		&_GameThread,
		self,
		0,
		&dwThreadID);

	
	WaitForSingleObject(hThread, INFINITE);
	GetExitCodeThread(hThread, &ExitCode);

	switch (ExitCode)
	{
	case 0:
	case 1:
		CloseHandle(hThread);
		break;
	default:
		CloseHandle(hThread);
		throw(new(Exception, __LINE__, __FILE__));
	}
}

constructMethodTable(
	.SetupScreen = _SetupScreen,
	.Start = _Start,
	.HandleInputEvents = virtual(HandleInputEvents),
	.Show = _Show
);

Constructor(selfptr, va_list* ap)
{
	account(self);
	assignMethodTable(this);

	this->AppName = "Veritas Engine Test";
	this->Window = new(ConsoleWindow);

	return this;
}
Destructor(selfptr)
{
	account(self);
	delete(this->Window);
	if(this->Control)
		delete(this->Control);
	if (this->Output)
		delete(this->Output);
	return this;
}
ENDCLASSDESC