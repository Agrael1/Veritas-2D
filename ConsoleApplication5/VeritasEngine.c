#include <stdlib.h>
#include "VeritasEngine.h"
#include "Class.h"

volatile BOOL bActive = false;

bool virtual(HandleInputEvents)(void* self, const KeyboardEvent event)
{
	if (event.type == Press)
	{
		// check if the event was for the escape key
		if (event.code == VK_ESCAPE)
		{
			return false;
		}
	}
	return true;
}
bool _PassEvents(selfptr)
{
	Optional(KeyboardEvent) event = { 0 };

	while ((event = self->Control.kbd.method->ReadKey(&self->Control.kbd)).bValid)
	{
		return self->method->HandleInputEvents(self, event.Value);
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
	int gResult = 0;
	Optional(WPARAM) message = {0};

	// Time counting
	LARGE_INTEGER StartingTime, EndingTime;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);

	if (!self->method->OnUserCreate(self))
		return 2;

	// first try
	QueryPerformanceCounter(&StartingTime);
	if (!self->method->OnUserUpdate(self, self->fElapsedSeconds))
	{
		gResult = 4;
		bActive = false;
	}
	else
	{
		bActive = true;
	}

	// Game Loop
	while (bActive)
	{
		QueryPerformanceCounter(&EndingTime);
		// Catch a focus if not in it
		if (!self->Control.bInFocus)
			self->Control.method->CatchFocus(&self->Control);

		// Read Messages
		if ((message = ProcessMessages()).bValid)
		{
			bActive = false;
			break;
		};

		// Process queue
		if (!_PassEvents(self))
		{
			gResult = 1;
			bActive = false;
			break;
		};

		// Process mouse
		if (self->method->HandleMouse)
			self->method->HandleMouse(self, &self->Control.mouse, self->fElapsedSeconds);

		// Process continuous input
		if (self->method->HandleControls)
			self->method->HandleControls(self, &self->Control.kbd, self->fElapsedSeconds);

		// render frame
		if (!self->method->OnUserUpdate(self, self->fElapsedSeconds))
		{
			gResult = 0;
			bActive = false;
			break;
		};

		self->fElapsedSeconds = (float)(EndingTime.QuadPart - StartingTime.QuadPart) / (float)Frequency.QuadPart;
		StartingTime = EndingTime;

		// present frame
		_Show(self);
	}

	if (self->method->OnUserDestroy&&!self->method->OnUserDestroy(self))
	{
		gResult = 3;
	}
	return gResult;
}
void _Start(selfptr)
{
	// parse game output (or not)
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
	delete_s(self->Output);
	deconstruct(&self->Control);
	deconstruct(&self->Window);
	return self;
}
ENDCLASSDESC