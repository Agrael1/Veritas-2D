#pragma once
#include "GDIPlusManager.h"

class Surface
{
public:
	Surface(const std::wstring& filepath);
public:
	UINT GetWidth()noexcept;
	UINT GetHeight()noexcept;
	Gdiplus::BitmapData& GetBufferPtr()noexcept;
	struct _BitmapData GetBufferPtrC();
private:
	Gdiplus::Bitmap bitmap;
	Gdiplus::BitmapData buffer;
};
