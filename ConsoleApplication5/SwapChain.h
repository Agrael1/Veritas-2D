#pragma once
#include "WinSetup.h"
#include "EngineCommons.h"

#define c_class SwapChain

class
{
	GENERATED_DESC

	CHAR_INFO* WriteFrame;
	CHAR_INFO* ReadFrame;
	float* ZBuffer;

	methods(
		bool (*DepthTest)(selfptr, unsigned x, unsigned y, float zValue);
		void (*ClearFrame)(selfptr, CHAR_INFO color);
		void (*BeginFrame)(selfptr, CHAR_INFO color);
		void (*PresentFrame)(selfptr);

		void (*DrawTriangleWireframe)(selfptr, int x1, int y1, int x2, int y2, int x3, int y3, CHAR_INFO color);
		void (*DrawLine)(selfptr, int x1, int y1, int x2, int y2, CHAR_INFO color);
		void (*PrintFrame)(selfptr, int x, int y, CHAR_INFO color); //do not use if unchecked (x,y)
		void (*DrawRectangle)(selfptr, int x1, int y1, int x2, int y2, unsigned short color);
	);

	unsigned short nFrameLength;
	unsigned short nFrameHeight;
};

