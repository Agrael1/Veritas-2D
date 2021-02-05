#pragma once
#include "WinSetup.h"
#include <RuntimeClass.h>
#include <stdbool.h>
#include <math.h>

#pragma push_macro("c_class")
#undef c_class
#define c_class SwapChain

typedef struct SwapChain SwapChain;

struct SwapChain
{
	CHAR_INFO* WriteFrame;	///< Frame that writing goes onto
	CHAR_INFO* ReadFrame;	///< Output read frame
	float* ZBuffer;			///< Depth buffer

	COORD Dimensions;		///< Dimensions of each buffer
};	

/// @brief Constructs 2 buffers and depth with provided dimensions 
/// @param Dimensions - size of buffers
void Constructor(selfptr, COORD Dimensions);
void Destructor(selfptr);

/// @brief Does depth test with provided coordinates (compares new value with old and stores new if less)
/// @param x - x coordinate of comparison 
/// @param y - y coordinate of comparison
/// @param zValue - new depth value
/// @return true if new value is less than previous value at (x,y)
inline bool DepthTest(selfptr, unsigned x, unsigned y, float zValue)
{
	float* zv = self->ZBuffer + y * self->Dimensions.X + x;
	if (zValue < *zv)
	{
		*zv = zValue;
		return true;
	}
	return false;
}

/// @brief Swaps read and write frames 
/// @return pointer to read buffer (the one to print to the console)
inline CHAR_INFO* PresentFrame(selfptr)
{
	CHAR_INFO* tmp = self->ReadFrame;
	self->ReadFrame = self->WriteFrame;
	self->WriteFrame = tmp;
	return self->ReadFrame;
}

/// @brief Places character(pixel) into write buffer at (x,y)
/// Note: (0, 0) is top left
/// @param x - x coordinate of the placement
/// @param y - y coordinate of the placement
/// @param color - pixel as a colored unicode/ASCII char
inline void PrintFrame(selfptr, unsigned x, unsigned y, CHAR_INFO color)
{
	self->WriteFrame[y * self->Dimensions.X + x] = color;
}

/// @brief Clears frame with one solid color
/// @param color - pixel as a colored unicode/ASCII char
inline void ClearFrame(selfptr, CHAR_INFO color)
{
#ifdef MEMSET32
	memset32(self->WriteFrame, *(unsigned*)&color, (size_t)self->Dimensions.X * self->Dimensions.Y);
#else
	CHAR_INFO* start = self->WriteFrame;
	CHAR_INFO* end = start + self->Dimensions.X * self->Dimensions.Y;
	while (start != end)
	{
		*start++ = color;
	}
#endif // MEMSET32
}

//TODO: no cleaning depth buffer
/// @brief Clears depth buffer with infinities 
inline void ClearDepth(selfptr)
{
#ifdef MEMSET32
	union
	{
		unsigned int i;
		float f;
	}imp = { .f = INFINITY };
	memset32(self->ZBuffer, imp.i, (size_t)self->Dimensions.X* self->Dimensions.Y);
#else
	float* start = self->ZBuffer;
	float* end = start + self->Dimensions.X * self->Dimensions.Y;
	while (start != end)
	{
		*start++ = INFINITY;
	}
#endif // MEMSET32
}

/// @brief Clears frame with color and resets depth
/// @param color - pixel as a colored unicode/ASCII char
inline void BeginFrame(selfptr, CHAR_INFO color)
{
	ClearFrame(self, color);
	ClearDepth(self);
}

// Old Functions
//void (*DrawTriangleWireframe)(selfptr, int x1, int y1, int x2, int y2, int x3, int y3, CHAR_INFO color);
//void (*DrawLine)(selfptr, int x1, int y1, int x2, int y2, CHAR_INFO color);
//void (*DrawRectangle)(selfptr, int x1, int y1, int x2, int y2, unsigned short color);

#ifndef SWAPCHAIN_IMPL
#pragma pop_macro("c_class")
#endif

