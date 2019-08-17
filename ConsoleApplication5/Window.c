#include "Class.h"
#include "StringStream.h"
#include "Window.h"
#include <malloc.h>

// namespace ConsoleWindow
#define c_class ConsoleWindow


bool _Restore(const selfptr)
{
	const account(self);
	if (!SetConsoleActiveScreenBuffer(this->hOriginalConsole))
		return false;
	return true;
}
bool _SetFont(const selfptr, Byte fontw, Byte fonth)
{
	const account(self);

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontw;
	cfi.dwFontSize.Y = fonth;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	wcscpy_s(cfi.FaceName, 9, L"Consolas");

	if (!SetCurrentConsoleFontEx(this->hOut, false, &cfi))
		return 0;

	return 1;
}
bool _SetCursor(selfptr, bool value)
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = value;
	if (!SetConsoleCursorInfo(self->hOut, &info))
		return false;
	return true;
}
COORD _CreateConsole(selfptr, Word width, Word height, Byte fontw, Byte fonth)
{
	account(self);

	SetWindowLongPtr(this->consoleWindow, GWL_STYLE,
		GetWindowLong(this->consoleWindow, GWL_STYLE)
		& ~WS_SIZEBOX & ~WS_SYSMENU & ~WS_MINIMIZEBOX);
	

	this->Height = height;
	this->Width = width;

	this->hOut = CreateConsoleScreenBuffer(
		GENERIC_READ |           // read/write access 
		GENERIC_WRITE,
		FILE_SHARE_READ |
		FILE_SHARE_WRITE,        // shared 
		NULL,                    // default security attributes 
		CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
		NULL);

	WND_CALL_INFO(this->hOut != INVALID_HANDLE_VALUE);
	WND_CALL_INFO(SetConsoleActiveScreenBuffer(this->hOut));
	WND_CALL_INFO(_SetFont(this, fontw, fonth));

	COORD coordLargest = GetLargestConsoleWindowSize(this->hOut);
	if (height > coordLargest.Y)
		this->Height = coordLargest.Y;
	if (width > coordLargest.X)
		this->Width =  coordLargest.X;
	coordLargest = (COORD) { this->Width, this->Height};

	// Set size of buffer
	WND_CALL_INFO(SetConsoleScreenBufferSize(this->hOut, coordLargest));

	// Set Physical Console Window Size
	this->rWindowRect = (SMALL_RECT){ 0, 0, (short)this->Width - 1, (short)this->Height - 1 };
	WND_CALL_INFO(SetConsoleWindowInfo(this->hOut, TRUE, &this->rWindowRect));
	return coordLargest;
}

void _OutputToScreen(selfptr, CHAR_INFO* buffer)
{
	WriteConsoleOutputW(self->hOut, buffer, (COORD) { (short)self->Width, (short)self->Height}, (COORD) { 0, 0 }, &self->rWindowRect);
}
void _SetPalette(selfptr, COLORREF palette[16])
{
	CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
	csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(self->hOut, &csbiex);
	csbiex.dwSize.Y--;					// a little fix of scrollbars
	memcpy(csbiex.ColorTable, palette, 16 * sizeof(COLORREF));
	SetConsoleScreenBufferInfoEx(self->hOut, &csbiex);
}

void _BlockCursor(selfptr)
{
	RECT rekt;
	GetWindowRect(self->consoleWindow, &rekt);
	rekt.right = rekt.left + 1;
	ClipCursor(&rekt);
}
void _ReleaseCursor(selfptr)
{
	ClipCursor(NULL);
}

constructMethodTable(
	.CreateConsole = _CreateConsole,
	.SetCursor = _SetCursor,
	.Restore = _Restore,
	.OutputToScreen = _OutputToScreen,
	
	.SetPalette = _SetPalette,
	.BlockCursor = _BlockCursor,
	.ReleaseCursor = _ReleaseCursor
);

Constructor(selfptr, va_list* ap)
{
	account(self);
	assignMethodTable(this);

	this->consoleWindow = GetConsoleWindow();
	this->hIn = GetStdHandle(STD_INPUT_HANDLE);
	this->hOriginalConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	this->hOut = this->hOriginalConsole;

	return this;
}
Destructor(selfptr)
{
	_Restore(self);
	return self;
}
ENDCLASSDESC

// namespace WindowException
#undef c_class
#define c_class WindowException

const char* virtual(GetType)()
{
	return "Console Window Exception\n\r";
}
char* TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuf = NULL;
	DWORD nMsgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsgBuf,
		0, NULL);

	if (nMsgLen == 0)
	{
		return "Unknown error";
	}
	return pMsgBuf;
}
char* GetErrorString(selfptr)
{
	account(self);
	return TranslateErrorCode(private.hr);
}
char* virtual(what)(selfptr)
{
	account(self);
	struct StringStream *oss = new(StringStream);
	char* _proxy = GetErrorString(this);
	char* _origin = base.method->GetOriginString(this);

	oss->method->Append(oss, base.method->GetType())
		->method->Append(oss, "[Error Code]: ")->method->AppendI(oss, private.hr)
		->method->Append(oss, "\n[Description]: ")->method->Append(oss, _proxy)->method->Append(oss,"\n")
		->method->Append(oss, _origin);

	LocalFree(_proxy);
	free(_origin);

	base.whatBuffer = oss->method->EndStr(oss);
	return base.whatBuffer;
}
Constructor(selfptr, va_list* ap)
{
	struct c_class *this = ((struct Class*)Exception)->ctor(self,ap);
	private.hr = va_arg(*ap, HRESULT);

	base.method->what = virtual(what);
	base.method->GetType = virtual(GetType);

	return this;
}
Destructor(selfptr)
{
	struct c_class *this = ((struct Class*)Exception)->dtor(self);
	return this;
}
ENDCLASSDESC 