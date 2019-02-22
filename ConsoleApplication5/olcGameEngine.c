#include "olcGameEngine.h"
#include <time.h>
#include <stdio.h>
#include "Class.h"

static const struct Class _olcGameEngine =
{
	sizeof(struct olcGameEngine), 
	olcGameEngine_ctor
};

const void* olcGameEngine = &_olcGameEngine;

DWORD _stdcall GameThread(void* _self);

int ConstructConsole(void* _self, int width, int heigh, int fontw, int fonth)
{
	struct olcGameEngine* this = _self;
	this->m_nScreenHeight = heigh;
	this->m_nScreenWidth = width;
	bool bMaxWindow = false;


	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontw;
	cfi.dwFontSize.Y = fonth;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Consolas");

	if (!SetCurrentConsoleFontEx(this->m_hConsole, bMaxWindow, &cfi))
		return -1;

	COORD coordLargest = GetLargestConsoleWindowSize(this->m_hConsole);
	if (this->m_nScreenHeight > coordLargest.Y)
		return -2;
	if (this->m_nScreenWidth > coordLargest.X)
		return -3;

	COORD buffer = { (short)this->m_nScreenWidth,(short)this->m_nScreenHeight };
	if (!SetConsoleScreenBufferSize(this->m_hConsole, buffer))
		return -4;
	this->m_rectWindow = (SMALL_RECT){ 0,0,(short)this->m_nScreenWidth - 1,(short)this->m_nScreenHeight - 1 };
	if (!SetConsoleWindowInfo(this->m_hConsole, TRUE, &this->m_rectWindow))
		return -5;

	this->m_bufScreen = malloc(this->m_nScreenHeight*this->m_nScreenWidth * sizeof(CHAR_INFO));
	return 0;
}

void Start(void* _self)
{
	DWORD dwThreadID;
	HANDLE hThread;

	struct olcGameEngine* this = _self;
	this->m_bAtomActive = true;

	hThread = CreateThread(
		NULL,
		0,
		&GameThread,
		_self,
		0,
		&dwThreadID);

	WaitForSingleObject(hThread, INFINITE);
}

DWORD _stdcall GameThread(void* _self)
{
	struct olcGameEngine* this = _self;

	clock_t tp1, tp2;
	/* Recording the starting clock tick.*/
	tp1 = clock();
	tp2 = clock();

	// Create user resources as part of this thread
	if (!this->OnUserCreate())
		this->m_bAtomActive = false;

	while (this->m_bAtomActive)
	{
		// Time handle
		tp2 = clock();
		clock_t elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = (float)(elapsedTime) / (float)(CLOCKS_PER_SEC);

		for (int i = 0; i < 256; i++)
		{
			this->m_keyNewState[i] = GetAsyncKeyState(i);

			this->m_keys[i].bPressed = false;
			this->m_keys[i].bReleased = false;

			if (this->m_keyNewState[i] != this->m_keyOldState[i])
			{
				if (this->m_keyNewState[i] & 0x8000)
				{
					this->m_keys[i].bPressed = !this->m_keys[i].bHeld;
					this->m_keys[i].bHeld = true;
				}
				else
				{
					this->m_keys[i].bPressed = true;
					this->m_keys[i].bHeld = false;
				}
			}
			this->m_keyOldState[i] = this->m_keyNewState[i];
		}
		// Handle frame update
		if (!this->OnUserUpdate(fElapsedTime))
			this->m_bAtomActive = false;
		WCHAR s[40];
		swprintf_s(s,40,L"Custom OLC Engine test-%3.2f", 1.0f / fElapsedTime);
		SetConsoleTitle(s);
		WriteConsoleOutputW(this->m_hConsole, this->m_bufScreen, (COORD){ (short)this->m_nScreenWidth, (short)this->m_nScreenHeight }, (COORD){ 0,0 }, &this->m_rectWindow);
	}

}