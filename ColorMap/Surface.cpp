#include "Surface.h"
#include <exception>
#include <sstream>
#include "GDIPMCalls.h"

#pragma comment( lib,"gdiplus.lib" )

Surface::Surface(const std::wstring& filepath)
	:bitmap(filepath.c_str())
{
	if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
	{
		std::stringstream ss;
		ss << "Loading image [" << std::string(filepath.begin(), filepath.end()) << "]: failed to load.";
		throw std::exception(ss.str().c_str());
	}
	bitmap.LockBits(&Gdiplus::Rect(0, 0, GetWidth(), GetHeight()), Gdiplus::ImageLockModeRead,
		PixelFormat32bppARGB, &buffer);
}

UINT Surface::GetWidth()noexcept
{
	return bitmap.GetWidth();
}
UINT Surface::GetHeight() noexcept
{
	return bitmap.GetHeight();
}

Gdiplus::BitmapData& Surface::GetBufferPtr() noexcept
{
	return buffer;
}
BitmapData Surface::GetBufferPtrC()
{
	BitmapData* p = reinterpret_cast<BitmapData*>(&this->buffer);
	return *p;
}

// C Style Interface Accessors
BitmapData GetImageFromFileA(const char* filename)
{
	std::string file(filename);
	std::wstring wfile(file.begin(), file.end());
	Surface S(wfile);
	return S.GetBufferPtrC();
}
BitmapData GetImageFromFileW(const wchar_t* filename)
{
	Surface S(filename);
	return S.GetBufferPtrC();
}