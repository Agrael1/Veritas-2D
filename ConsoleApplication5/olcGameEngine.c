#include "olcGameEngine.h"
#include <time.h>
#include <stdio.h>
#include "Class.h"

// Internal functions. Paste functions here:
// Clipping wrap
void Clip(void* self, int *x, int *y)
{
	struct olcGameEngine* this = self;
	if (*x < 0) *x = 0;
	if (*x >= this->m_nScreenWidth) *x = this->m_nScreenWidth;
	if (*y < 0) *y = 0;
	if (*y >= this->m_nScreenHeight) *y = this->m_nScreenHeight;
}

// Fill function
void Fill(void* _self, int x1, int y1, int x2, int y2, wchar_t sym, short color)
{
	struct olcGameEngine* this = _self;
	Clip(this, &x1, &y1);
	Clip(this, &x2, &y2);
	{
		for (int x = x1; x < x2; x++)
			for (int y = y1; y < y2; y++)
			{
				this->m_bufScreen[y*this->m_nScreenWidth + x].Char.UnicodeChar = sym;
				this->m_bufScreen[y*this->m_nScreenWidth + x].Attributes = color;
			}
	}
}
// Game thread function
DWORD _stdcall GameThread(void* _self)
{
	struct olcGameEngine* this = _self;

	clock_t tp1, tp2;
	/* Recording the starting clock tick.*/
	tp1 = clock();
	tp2 = clock();

	// Create user resources as part of this thread
	if (!this->OnUserCreate(this))
		this->m_bAtomActive = false;

	while (this->m_bAtomActive)
	{
		// Time handle
		tp2 = clock();
		float fElapsedTime = (float)(tp2 - tp1) / (float)(CLOCKS_PER_SEC);
		tp1 = tp2;
		

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
		if (!this->OnUserUpdate(this,fElapsedTime))
			this->m_bAtomActive = false;
		WCHAR s[40];
		swprintf_s(s,40,L"Custom OLC Engine test-%3.2f", 1.0 / fElapsedTime);
		SetConsoleTitle(s);
		WriteConsoleOutputW(this->m_hConsole, this->m_bufScreen, (COORD){ (short)this->m_nScreenWidth, (short)this->m_nScreenHeight }, (COORD){ 0,0 }, &this->m_rectWindow);
	}
	return 0;
}
// Constructs console with input params
int ConstructConsole(void* _self, int width, int heigh, int fontw, int fonth)
{
	struct olcGameEngine* this = _self;
	this->m_nScreenHeight = heigh;
	this->m_nScreenWidth = width;
	bool bMaxWindow = false;

	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(this->m_hConsole, &info);

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

	// init buffer
	if ((*this).m_bufScreen)
		Fill(this, 0, 0, this->m_nScreenWidth, this->m_nScreenHeight, L' ', BG_BLACK);

	return 0;
}
// Start rouitine, assembles a thread and gives it a handle
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
// Symbolic print function
void Printscr(void* _self, int x, int y, wchar_t character, short color)
{
	struct olcGameEngine* this = _self;
	if (x >= 0 && x < this->m_nScreenWidth&&y >= 0 && y < this->m_nScreenHeight)
	{
		this->m_bufScreen[y*this->m_nScreenWidth + x].Char.UnicodeChar = character;
		this->m_bufScreen[y*this->m_nScreenWidth + x].Attributes = color;
	}
}

void FillCenter(void* _self, int DimX, int DimY, wchar_t sym, short color)
{
	struct olcGameEngine* this = _self;
	Clip(this, &DimX, &DimY);

	for(int y = (int)((this->m_nScreenHeight-DimY)/2);y<(this->m_nScreenHeight - DimY) / 2+DimY; y++)
		for (int x = (int)((this->m_nScreenWidth - DimX) / 2); x < (this->m_nScreenWidth - DimX) / 2 + DimX; x++)
		{
			this->m_bufScreen[y*this->m_nScreenWidth + x].Char.UnicodeChar = sym;
			this->m_bufScreen[y*this->m_nScreenWidth + x].Attributes = color;
		}
}

// Constructor (must be last to bind methods)
void* olcGameEngine_ctor(void* _self, va_list *app)
{
	struct olcGameEngine* this = _self;

	this->m_nScreenWidth = 80;
	this->m_nScreenHeight = 30;

	this->m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	this->m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	this->m_keys = (struct sKeyState*)calloc(256, sizeof(struct sKeyState));

	this->m_bEnableSound = false;

	this->ConstructConsole = ConstructConsole;
	this->Start = Start;
	this->Printscr = Printscr;
	this->Fill = Fill;
	this->FillCenter = FillCenter;
	return this;
}
// Destructor
void* olcGameEngine_dtor(void* self)
{
	struct olcGameEngine *this = self;
	free(this->m_keys);

	SetConsoleActiveScreenBuffer(this->m_hOriginalConsole);
	free(this->m_bufScreen);
	return this;
}

static const struct Class _olcGameEngine =
{
	sizeof(struct olcGameEngine), 
	olcGameEngine_ctor, olcGameEngine_dtor
};
const void* olcGameEngine = &_olcGameEngine;