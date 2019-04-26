#include "Class.h"
#include "VeritasEngine.h"

volatile bActive = false;

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
		ShowCursor(false);
		return true;
	}
		
	else
		return false;
}

DWORD _stdcall _GameThread(void* _self)
{
	struct c_class* this = _self;
	this->Control = new(MessageWindow, this->Window->consoleWindow);
	int gResult;

	// Game Loop
	while (true)
	{
		if (gResult = ProcessMessages() != 0)
			return gResult;
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
	delete(this->Control);
	if (this->Output)
		delete(this->Output);
	return this;
}
ENDCLASSDESC