#pragma once
#include "WinSetup.h"
#include "EngineCommons.h"
#include "VMathTypes.h"

#define c_class Frame

class
{
	GENERATED_DESC

	CHAR_INFO *localFrame;
	float* ZBuffer;

	Word nFrameLength;
	Word nFrameHeight;

	methods(
		bool (*DepthTest)(selfptr, unsigned x, unsigned y, float zValue);
		void (*ClearFrame)(selfptr, wchar_t c, Word col);
		void (*BeginFrame)(selfptr, wchar_t c, Word col);
		void (*DrawTriangleWireframe)(selfptr, Word x1, Word y1, Word x2, Word y2, Word x3, Word y3, wchar_t c, Word col);
		void (*DrawTriangle)(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2, wchar_t c, Word col);
		void (*DrawLine)(selfptr, Word x1, Word y1, Word x2, Word y2, wchar_t character, Word col);
		void (*PrintFrame)(selfptr, Word x, Word y, CHAR_INFO color);
		void (*DrawRectangle)(selfptr, Word x1, Word y1, Word x2, Word y2, Word color);
	);
	// 3D stuff
	VMMATRIX projection;
	VMMATRIX camera;
};

