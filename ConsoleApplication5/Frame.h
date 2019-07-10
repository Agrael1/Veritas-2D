#pragma once
#include "WinSetup.h"
#include "EngineCommons.h"
#include "VTypes.h"
#include "Color.h"

#define c_class Frame

class
{
	const void* _class;

	CHAR_INFO *localFrame;
	Word nFrameLength;
	Word nFrameHeight;

	methods(
		void (*ClearFrame)(selfptr, wchar_t c, Word col);
		void (*DrawTriangleWireframe)(selfptr, Word x1, Word y1, Word x2, Word y2, Word x3, Word y3, wchar_t c, Word col);
		void (*DrawTriangle)(selfptr, VMVECTOR* v0, VMVECTOR* v1, VMVECTOR* v2, wchar_t c, Word col);
		void (*DrawLine)(selfptr, Word x1, Word y1, Word x2, Word y2, wchar_t character, Word col);
		void (*PrintFrame)(void* _self, Word x, Word y, wchar_t character, Word color);
		void (*DrawRectangle)(void* self, Word x1, Word y1, Word x2, Word y2, Word color);
		void (*Compose)(void* self, struct Frame* localBuffer, Word offsetX, Word offsetY);
	);
	// 3D stuff
	VMMATRIX projection;
	VMMATRIX camera;
};

