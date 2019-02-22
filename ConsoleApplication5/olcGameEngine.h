#ifndef OLCGAMEENGINE
#define OLCGAMEENGINE

#include <Windows.h>

#define false 0;
#define true 1;
typedef char bool;

enum COLOUR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007, 
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};

struct sKeyState
{
	bool bPressed;
	bool bReleased;
	bool bHeld;
};

extern const void* olcGameEngine;

struct olcGameEngine 
{
	const void* class;
	int m_nScreenWidth;
	int m_nScreenHeight;
	CHAR_INFO *m_bufScreen;
	HANDLE m_hOriginalConsole;
	CONSOLE_SCREEN_BUFFER_INFO m_OriginalConsoleInfo;
	HANDLE m_hConsole;
	HANDLE m_hConsoleIn;
	SMALL_RECT m_rectWindow;
	short m_keyOldState[256];
	short m_keyNewState[256];
	bool m_bConsoleInFocus;
	bool m_bEnableSound;
	bool m_bAtomActive;

	struct sKeyState *m_keys;

	bool (*OnUserCreate)();
	bool (*OnUserUpdate)(float fElapsedTime);
	// Optional for clean up 
	bool(*OnUserDestroy)();

	int(*ConstructConsole)(void* _self, int width, int heigh, int fontw, int fonth);
	void(*Start)(void* _self);
};


static void* olcGameEngine_ctor(void* _self, va_list *app)
{
	struct olcGameEngine* this = _self;

	int ConstructConsole(void* _self, int width, int heigh, int fontw, int fonth);
	void Start(void* _self);

	this->ConstructConsole = ConstructConsole;
	this->Start = Start;

	this->m_nScreenWidth = 80;
	this->m_nScreenHeight = 30;

	this->m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	this->m_hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	*this->m_keyNewState = (short)malloc(256 * sizeof(short));
	*this->m_keyOldState = (short)malloc(256 * sizeof(short));
	this->m_keys = (struct sKeyState*)calloc(256, sizeof(struct sKeyState));

	this->m_bEnableSound = false;

	return this;
}
#endif // !OLCGAMEENGINE


