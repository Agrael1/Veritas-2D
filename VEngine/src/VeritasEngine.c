#define VENGINE_IMPL
#include "VeritasEngine.h"
#include <process.h>


static bool empty_func() { return true; }

extern inline void Show(selfptr);

// False makes application quit
bool Virtual(HandleInputEvents)(void* self, const KeyboardEvent event)
{
	return !(event.type == Press && event.code == VK_ESCAPE);
}
bool PassEvents(selfptr)
{
	Optional(KeyboardEvent) event = { 0 };

	while ((event = ReadKey(&self->Control.kbd)).bValid) {
		return self->method->HandleInputEvents(self, event.Value);
	}
	return true;
}

void _CreateDevice(selfptr)
{
	//self->pDevice = new(VLine, self->pSwap);
}
void _CreateDeviceAndSwapChain(selfptr, VConsoleDesc VPDesc)
{
	_CreateDevice(self);
}
void GameThread(selfptr)
{
	// Time counting
	LARGE_INTEGER StartingTime, EndingTime;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);

	if (!self->method->OnUserCreate(self))
		return;

	QueryPerformanceCounter(&StartingTime);
	Sleep(16); //usual time for 60 fps, not to overdrive the input

	// Game Loop
	while (self->bActive)
	{
		if (!self->Control.bInFocus)
		{
			self->bSleeping = true;
			WaitForSingleObject(self->Holder, INFINITE);
			self->bSleeping = false;
		}

		QueryPerformanceCounter(&EndingTime);
		self->fElapsedSeconds = (float)(EndingTime.QuadPart - StartingTime.QuadPart) / (float)Frequency.QuadPart;
		StartingTime = EndingTime;

		// Process queue and render to a frame
		if (!PassEvents(self) 
			|| !self->method->OnUserUpdate(self, self->fElapsedSeconds))break;

		self->method->HandleMouse(self, &self->Control.mouse, self->fElapsedSeconds);
		self->method->HandleControls(self, &self->Control.kbd, self->fElapsedSeconds);

		// present frame
		Show(self);
	}

	self->method->OnUserDestroy(self);
	self->bActive = false; //ensure loop ends on false
	self->GraphicsThread = 0;
}

void Start(selfptr)
{
	self->bSleeping = false;
	self->bActive = true;
	Optional(WPARAM) message = { 0 };
	self->GraphicsThread = _beginthread(GameThread, 0, self);
	// parse game output (or not)
	while (self->bActive)
	{
		if ((message = ProcessMessages()).bValid)
		{
			self->bActive = false;
			break;
		}
		if (!self->Control.bInFocus)
		{
			ResetEvent(self->Holder);
			while (!&self->bSleeping); // Spinlock

			WaitMessage();
			if (CatchFocus(&self->Window))
			{
				SetEvent(self->Holder);
				ControlAwake(&self->Control);
			}
		}
	}
	if (self->GraphicsThread)
		WaitForSingleObject(self->GraphicsThread, INFINITE);
}

VirtualTable(IVeritasEngine) {
	.HandleInputEvents = Virtual(HandleInputEvents),
	.HandleControls = empty_func,
	.OnUserCreate = empty_func,
	.OnUserUpdate = empty_func,
	.OnUserDestroy = empty_func,
	.HandleMouse = empty_func
};

void Constructor(selfptr, VConsoleDesc screenparams)
{
	self->method = &vfptr;
	self->fElapsedSeconds = 0.0f;
	self->Holder = CreateEvent(NULL, true, true, NULL);

	ConsoleWindow_ctor(&self->Window);
	COORD frame = CreateConsole(&self->Window, screenparams.width, screenparams.height, screenparams.fontw, screenparams.fonth);
	SwapChain_ctor(&self->Swap, frame);
	MessageWindow_ctor(&self->Control, frame.X, frame.Y, self->Window.ConsoleWindow);
}
void Destructor(selfptr)
{
	MessageWindow_dtor(&self->Control);
	SwapChain_dtor(&self->Swap);
	ConsoleWindow_dtor(&self->Window);
}