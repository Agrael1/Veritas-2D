#include "Class.h"
#include "VeritasEngine.h"

#define EMPTYQUEUE this->Control->kbd->method->KeyIsEmpty(this->Control->kbd)
#define ReadEventQueue() this->Control->kbd->method->ReadKey(this->Control->kbd)

volatile bActive = false;


bool _OnUserUpdate(void* self)
{
	account(self);
	while (!EMPTYQUEUE)
	{
		// get an event from the queue
		const struct KeyboardEvent* e = ReadEventQueue();
		// check if it is a release event
		if (e->method->IsPress(e))
		{
			// check if the event was for the space key
			if (e->method->GetCode(e) == VK_ESCAPE)
			{
				return false;
			}
		}
	}
	return true;
}


void _Show(void* self)
{
	struct c_class* this = self;
	this->Window->method->OutputToScreen(this->Window, this->Output->localFrame);
}
bool _SetupScreen(void* self, Word width, Word height, Byte fontw, Byte fonth)
{
	struct c_class* this = self;

	// default setup for fast access
	if (this->Window->method->CreateConsole(this->Window, width, height, fontw, fonth)
		&& this->Window->method->SetCursor(this->Window, false))
	{
		this->Output = new(Frame, width, height);
		SetConsoleTitleA(this->AppName);
		_Show(this);
		return true;
	}
		
	else
		return false;
}
DWORD _stdcall _GameThread(void* _self)
{
	account(_self);
	this->Control = new(MessageWindow, this->Window->consoleWindow);
	int gResult;
	ShowCursor(false);

	// Game Loop
	while (true)
	{
		if (gResult = ProcessMessages() != 0)
			return gResult;
		
		// Process queue
		if (!_OnUserUpdate(this))
		{
			return 1;
		}
		// render frame
	}
	return 0;
}

void _Start(void* _self)
{
	DWORD dwThreadID;
	HANDLE hThread;

	struct c_class* this = _self;
	bActive = true;		


	hThread = CreateThread(
		NULL,
		0,
		&_GameThread,
		_self,
		0,
		&dwThreadID);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
}

constructMethodTable(
	.SetupScreen = _SetupScreen,
	.Start = _Start
);

Constructor(void* self, va_list* ap)
{
	struct c_class* this = self;
	assignMethodTable(this);

	this->AppName = "Veritas Engine Test";
	this->Window = new(ConsoleWindow);

	return this;
}
Destructor(void* self)
{
	struct c_class* this = self;
	delete(this->Window);
	if(this->Control)
		delete(this->Control);
	if (this->Output)
		delete(this->Output);
	return this;
}
ENDCLASSDESC