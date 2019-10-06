#include <stdlib.h>
#include "VeritasEngine.h"
#include "Class.h"

#define VTHREAD __stdcall

volatile BOOL bActive = false;

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
	self->Window->method->OutputToScreen(self->Window, self->Output->ReadFrame);
}
void _SetupScreen(selfptr, Word width, Word height, Byte fontw, Byte fonth)
{
	account(self);

	// default setup for fast access
	COORD frame = this->Window->method->CreateConsole(this->Window, width, height, fontw, fonth);

	this->Output = new(SwapChain, frame.X, frame.Y);
	_Show(this);
}
DWORD VTHREAD GameThread(selfptr)
{
	account(self);
	this->Control = new(MessageWindow, this->Window);
	int gResult;

	if (!this->method->OnUserCreate(this))
		return 2;

	// Time counting
	LARGE_INTEGER StartingTime, EndingTime;
	LARGE_INTEGER Frequency;
	double fElapsedSeconds = 0.0;
	int rtime = 0;
	int middle = 0, k = 0;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	// first try
	if (!this->method->OnUserUpdate(this, fElapsedSeconds))
	{
		gResult = 4;
		bActive = false;
	};

	bActive = true;
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
		{
			InterlockedAnd(&bActive, false);
			break;
		};

		// Process queue
		if (!_PassEvents(this))
		{
			gResult = 1;
			InterlockedAnd(&bActive, false);
			break;
		};

		// Process mouse
		if (this->method->HandleMouse)
			this->method->HandleMouse(this, &this->Control->mouse, fElapsedSeconds);

		// Process continuous input
		if (this->method->HandleControls)
			this->method->HandleControls(this, &this->Control->kbd, fElapsedSeconds);

		// render frame
		if (!this->method->OnUserUpdate(this, fElapsedSeconds))
		{
			gResult = 0;
			InterlockedAnd(&bActive, false);
			break;
		};
		//// FPS Counter
		//if (k++ == 60)
		//{
		//	char* buf = alloca(10);
		//	rtime = (int)(1.0 / fElapsedSeconds);
		//	middle = (rtime + middle) / 2;
		//	k = 0;
		//	SetConsoleTitleA(_itoa(middle, buf, 10));
		//}
	}

	if (this->method->OnUserDestroy&&!this->method->OnUserDestroy(this))
	{
		gResult = 3;
	}

	return gResult;
}
DWORD VTHREAD Render(selfptr)
{
	while (!bActive); //easy sync
	while (bActive)
	{
		WaitForSingleObject(self->Output->hSemaphore, INFINITE);
		self->Window->method->OutputToScreen(self->Window, self->Output->ReadFrame);
		ReleaseSemaphore(self->Output->hSemaphore, 1, nullptr);
	}
	return 0;
}
void _Start(selfptr)
{
	DWORD dwThreadID[2];
	HANDLE hRender, hGame;
	DWORD ExitCode;
	
	// start a game thread
	hGame = CreateThread(
		NULL,
		0,
		&GameThread,
		self,
		0,
		&dwThreadID[0]);

	hRender = CreateThread(
		NULL,
		0,
		&Render,
		self,
		0,
		&dwThreadID[1]);

	
	WaitForMultipleObjects(2u, (const HANDLE[]){hRender, hGame}, TRUE, INFINITE);
	GetExitCodeThread(hGame, &ExitCode);

	switch (ExitCode)
	{
	case 0:
	case 1:
		CloseHandle(hGame);
		CloseHandle(hRender);
		break;
	default:
		CloseHandle(hGame);
		CloseHandle(hRender);
		throw(new(Exception, __LINE__, __FILE__));
	}
}

VirtualTable{
	.SetupScreen = _SetupScreen,
	.Start = _Start,
	.HandleInputEvents = virtual(HandleInputEvents),
	.Show = _Show
};
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
	delete_s(this->Output);
	delete_s(this->Control);
	delete(this->Window);
	return this;
}
ENDCLASSDESC