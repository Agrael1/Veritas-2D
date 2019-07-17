#pragma once
#include "Color.scheme"
#include "EngineCommons.h"

struct PSOut
{
	Word sym;
	Word color;
};

// Saturation Color picker:
// @color - BG_...
// @sat - value from 0 to 8, Ambient light is not supported right now
// @satColor - FG_...
inline struct PSOut Pick(Word color, Byte sat, Byte satColor)
{
	switch (sat)
	{
	case 0: return (struct PSOut) { PIXEL_NONE, BG_Black };
	case 1: return (struct PSOut) { PIXEL_THREEQUARTERS, color | FG_Black };
	case 2: return (struct PSOut) { PIXEL_HALF, color | FG_Black };
	case 3: return (struct PSOut) { PIXEL_QUARTER, color | FG_Black };
	case 4: return (struct PSOut) { PIXEL_NONE, color };
	case 5: return (struct PSOut) { PIXEL_QUARTER, color|satColor };
	case 6: return (struct PSOut) { PIXEL_HALF, color | satColor};
	case 7: return (struct PSOut) { PIXEL_THREEQUARTERS, color | satColor };
	case 8: return (struct PSOut) { PIXEL_SOLID, color | satColor };
	}
}