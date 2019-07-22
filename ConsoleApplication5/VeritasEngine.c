#include "Class.h"
#include <stdlib.h>
#include "StringStream.h"
#include "VeritasEngine.h"

#define EMPTYQUEUE this->Control->kbd.method->KeyIsEmpty(&this->Control->kbd)
#define ReadEventQueue(event) struct KeyboardEvent* __event_ptr = this->Control->kbd.method->ReadKey(&this->Control->kbd);\
event = *__event_ptr; delete(__event_ptr)


__declspec(thread) char buf[10];
volatile bool bActive = false;

bool virtual(HandleInputEvents)(void* self, struct KeyboardEvent event)
{
	if (event.method->IsPress(&event))
	{
		// check if the event was for the space key
		if (event.method->GetCode(&event) == VK_ESCAPE)
		{
			return false;
		}
	}
	return true;
}
bool _PassEvents(void* self)
{
	account(self);
	struct KeyboardEvent event;

	while (!EMPTYQUEUE)
	{
		// get an event from the queue
		ReadEventQueue(event);
		return this->method->HandleInputEvents(this, event);
	}
	return true;
}

void _Show(selfptr)
{
	self->Window->method->OutputToScreen(self->Window, self->Output->localFrame);
}
bool _SetupScreen(selfptr, Word width, Word height, Byte fontw, Byte fonth)
{
	account(self);

	// default setup for fast access
	if (this->Window->method->CreateConsole(this->Window, width, height, fontw, fonth))
	{
		this->Output = new(Frame, width, height);
		SetConsoleTitleA(this->AppName);
		_Show(this);

		return true;
	}
	return false;
}
DWORD _stdcall _GameThread(selfptr)
{
	account(self);
	this->Control = new(MessageWindow, this->Window->consoleWindow);
	int gResult;

	if (!this->method->OnUserCreate(this))
		return 2;

	// Time counting
	LARGE_INTEGER StartingTime, EndingTime;
	LARGE_INTEGER Frequency;
	double fElapsedSeconds;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	// Game Loop
	while (bActive)
	{
		QueryPerformanceCounter(&EndingTime);
		fElapsedSeconds = (double)(EndingTime.QuadPart - StartingTime.QuadPart) / (double)Frequency.QuadPart;
		StartingTime = EndingTime;

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
		if(this->method->HandleControls)
			this->method->HandleControls(this, &this->Control->kbd, fElapsedSeconds);

		// render frame
		if (!this->method->OnUserUpdate(this, fElapsedSeconds))
			return 0;

		_Show(this);

		SetConsoleTitleA(_itoa((int)(1.0 / fElapsedSeconds),buf,10));
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