#pragma once
#include "WinSetup.h"
#include "EngineCommons.h"
#include "VTypes.h"

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

#define c_class Frame

class
{
	const void* _class;

	CHAR_INFO *localFrame;
	Word nFrameLength;
	Word nFrameHeight;

	methods(
		void (*DrawTriangle)(selfptr, Word x1, Word y1, Word x2, Word y2, Word x3, Word y3, wchar_t c, Word col);
		void (*DrawLine)(selfptr, Word x1, Word y1, Word x2, Word y2, wchar_t character, Word col);
		void (*PrintFrame)(void* _self, Word x, Word y, wchar_t character, Word color);
		void (*DrawRectangle)(void* self, Word x1, Word y1, Word x2, Word y2, Word color);
		void (*Compose)(void* self, struct Frame* localBuffer, Word offsetX, Word offsetY);
	);
	// 3D stuff
	VMMATRIX projection;
};

