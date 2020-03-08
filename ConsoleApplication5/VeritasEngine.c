#include <stdlib.h>
#include "VeritasEngine.h"
#include "Class.h"

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
	self->Window.method->OutputToScreen(&self->Window);
}
void _SetupScreen(selfptr, Word width, Word height, Byte fontw, Byte fonth)
{
	// default setup for fast access
	COORD frame = self->Window.method->CreateConsole(&self->Window, width, height, fontw, fonth);

	self->Output = new(SwapChain, frame.X, frame.Y);
	self->Window.ppBuffer = &self->Output->ReadFrame;
	_Show(self);
}
DWORD GameThread(selfptr)
{
	account(self);
	int gResult;

	if (!this->method->OnUserCreate(this))
		return 2;

	// Time counting
	int rtime = 0;
	int middle = 0, k = 0;

	LARGE_INTEGER StartingTime, EndingTime;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	// first try
	if (!this->method->OnUserUpdate(this, this->fElapsedSeconds))
	{
		gResult = 4;
		bActive = false;
	};

	bActive = true;
	// Game Loop

	while (bActive)
	{
		QueryPerformanceCounter(&EndingTime);
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
			this->method->HandleMouse(this, &this->Control->mouse, this->fElapsedSeconds);

		// Process continuous input
		if (this->method->HandleControls)
			this->method->HandleControls(this, &this->Control->kbd, this->fElapsedSeconds);

		// render frame
		if (!this->method->OnUserUpdate(this, this->fElapsedSeconds))
		{
			gResult = 0;
			InterlockedAnd(&bActive, false);
			break;
		};

		self->fElapsedSeconds = (float)(EndingTime.QuadPart - StartingTime.QuadPart) / (float)Frequency.QuadPart;
		StartingTime = EndingTime;

		self->Window.method->OutputToScreen(&self->Window);
	}

	if (this->method->OnUserDestroy&&!this->method->OnUserDestroy(this))
	{
		gResult = 3;
	}

	return gResult;
}
void _Start(selfptr)
{
	GameThread(self);
}

VirtualTable{
	.SetupScreen = _SetupScreen,
	.Start = _Start,
	.HandleInputEvents = virtual(HandleInputEvents),
	.Show = _Show
};
Constructor(selfptr, va_list* ap)
{
	assignMethodTable(self);
	self->AppName = "Veritas Engine Test";
	construct(&self->Window, ConsoleWindow);
	construct(&self->Control, MessageWindow, &self->Window);

	return self;
}
Destructor(selfptr)
{
	account(self);
	delete_s(this->Output);
	deconstruct(&this->Control);
	deconstruct(&this->Window);
	return this;
}
ENDCLASSDESC