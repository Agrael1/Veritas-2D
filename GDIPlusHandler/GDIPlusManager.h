#pragma once
#define FULL_WINOPT
#include "..\ConsoleApplication5\WinSetup.h"
#include <algorithm>
#include <string>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>

class GDIPlusManager
{
public:
	GDIPlusManager();
	~GDIPlusManager();
private:
	static ULONG_PTR token;
	static int refCount;
};