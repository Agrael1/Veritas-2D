#pragma once
#include "WinSetup.h"
#include "Color.scheme"
#include "EngineCommons.h"


// Saturation Color picker:
// @color - BG_...
// @sat - value from 0 to 8, Ambient light is not supported right now
// @satColor - FG_...
inline CHAR_INFO Pick(Word color, Byte sat, Byte satColor)
{
	switch (sat)
	{
	default:
	case 0: return (CHAR_INFO) { PIXEL_NONE, BG_Black };
	case 1: return (CHAR_INFO) { PIXEL_THREEQUARTERS, color | FG_Black };
	case 2: return (CHAR_INFO) { PIXEL_HALF, color | FG_Black };
	case 3: return (CHAR_INFO) { PIXEL_QUARTER, color | FG_Black };
	case 4: return (CHAR_INFO) { PIXEL_NONE, color };
	case 5: return (CHAR_INFO) { PIXEL_QUARTER, color|satColor };
	case 6: return (CHAR_INFO) { PIXEL_HALF, color | satColor};
	case 7: return (CHAR_INFO) { PIXEL_THREEQUARTERS, color | satColor };
	case 8: return (CHAR_INFO) { PIXEL_SOLID, color | satColor };

	}
}