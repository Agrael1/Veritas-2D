#ifndef OLCGAMEENGINE
#define OLCGAMEENGINE

#include <Windows.h>
#include "EngineCommons.h"

#define c_class olcGameEngine

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
	bool bPressed:1;
	bool bReleased:1;
	bool bHeld:1;
};

extern const void* olcGameEngine;

typedef struct
{
	// Virtual functions 

		// Creation handling
		bool(*OnUserCreate)(void* self);
		// Frame handling
		// *self - class pointer input
		// float fElapsedTime - time input for controls
		bool(*OnUserUpdate)(void* self, float fElapsedTime);
		// Optional for clean up *unused*
		bool(*OnUserDestroy)();

	// Internal class functions

		// Constructs console with input params:
		// _self - class pointer input
		// width, height - width and height of a console
		// fontw, fonth - width and height of the font on screen
		int(*ConstructConsole)(void* _self, int width, int heigh, int fontw, int fonth);
		// Start routine
		// *_self - class pointer input
		void(*Start)(void* _self);
		// Prints character on the screen
		// *_self - Game engine class pointer
		// x, y - coordinates of input charachter
		void(*PrintChar)(void* _self, int x, int y, wchar_t character, short color);

		int (*PrintStringW)(_Inout_updates_(m_buffscreen) void* _self, int x, int y, _In_reads_(length) const wchar_t *strptr, unsigned int length, short color);
		// Makes a part of a screen filled with specific character
		void(*Fill)(void* _self, int x1, int y1, int x2, int y2, wchar_t sym, short color);
		// Fills a center part of screen with symbol
		// Dimx, DimY - dimensions of center segment
		void(*FillCenter) (void* _self, int DimX, int DimY, wchar_t sym, short color);
		// Draw rectangle with 4 coordinates 
		// @ x1, y1 - from
		// @ x2, y2 - to
		// all the coordinates can be in random relation
		void (*DrawRectangle)(void* self, unsigned x1, unsigned y1, unsigned x2, unsigned y2, unsigned short color);

}vftb;

struct olcGameEngine 
{
	const void* class;
	vftb* method;

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
	bool m_bConsoleInFocus:1;
	bool m_bEnableSound:1;
	bool m_bAtomActive:1;

	struct sKeyState *m_keys;
};
#endif // !OLCGAMEENGINE


