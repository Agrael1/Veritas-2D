#include "GDIPlusManager.h"
#include "GDIPMCalls.h"

ULONG_PTR GDIPlusManager::token = 0;
int GDIPlusManager::refCount = 0;

GDIPlusManager::GDIPlusManager()
{
	if (refCount++ == 0)
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup(&token, &input, nullptr);
	}
}
GDIPlusManager::~GDIPlusManager()
{
	if (--refCount == 0)
	{
		Gdiplus::GdiplusShutdown(token);
	}
}

// C Style Interface Accessors
HGDIPM GDIPMStart()
{
	return reinterpret_cast<HGDIPM>(new GDIPlusManager());
}
void GDIPMStop(HGDIPM handle)
{
	delete reinterpret_cast<GDIPlusManager*>(handle);
}