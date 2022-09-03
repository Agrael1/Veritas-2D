#define VENGINE_IMPL
#include "VeritasEngine.h"
#include "Class.h"

volatile BOOL bActive = false;

bool virtual(HandleInputEvents)(void* self, const KeyboardEvent event)
{
	if (event.type == Press && event.code == VK_ESCAPE)
		PostQuitMessage(0);
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
void _CreateDevice(selfptr)
{
	self->pDevice = new(VLine, self->pSwap);
}
void _CreateSwapChain(selfptr, VConsoleDesc VPDesc)
{
	// default setup for fast access
	COORD frame = self->Window.method->CreateConsole(&self->Window, VPDesc.width, VPDesc.height, VPDesc.fontw, VPDesc.fonth);

	self->pSwap = new(SwapChain, frame.X, frame.Y);
	self->Window.ppBuffer = &self->pSwap->ReadFrame;
	_Show(self);
}
void _CreateDeviceAndSwapChain(selfptr, VConsoleDesc VPDesc)
{
	_CreateSwapChain(self, VPDesc);
	_CreateDevice(self);
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
	.CreateSwapChain = _CreateSwapChain,
	.CreateDeviceAndSwapChain = _CreateDeviceAndSwapChain,
	.Start = _Start,
	.HandleInputEvents = virtual(HandleInputEvents),
	.Show = _Show
};
Constructor(selfptr, va_list* ap)
{
	InitializeVtable(self);
	self->AppName = "Veritas Engine Test";
	self->pDevice = NULL;
	construct(&self->Window, ConsoleWindow);
	construct(&self->Control, MessageWindow, &self->Window);
}
Destructor(selfptr)
{
	delete_s(self->pSwap);
	delete_s(self->pDevice);
	deconstruct(&self->Control);
	deconstruct(&self->Window);
}
ENDCLASSDESC