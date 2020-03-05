#include "Class.h"
#include "Window.h"

// namespace ConsoleWindow
#define c_class ConsoleWindow

bool _Restore(const selfptr)
{
	SetWindowLongPtr(self->consoleWindow, GWL_STYLE, self->lOriginalParams);
	return (bool)SetConsoleActiveScreenBuffer(self->hOriginalConsole);
}
bool _SetFont(const selfptr, Byte fontw, Byte fonth)
{
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontw;
	cfi.dwFontSize.Y = fonth;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, 9, L"Consolas");

	return (bool)SetCurrentConsoleFontEx(self->hOut, false, &cfi);
}
bool _SetCursor(selfptr, bool value)
{
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = value;
	return (bool)SetConsoleCursorInfo(self->hOut, &info);
}
COORD _CreateConsole(selfptr, Word width, Word height, Byte fontw, Byte fonth)
{
	self->lOriginalParams = GetWindowLong(self->consoleWindow, GWL_STYLE);
	SetWindowLongPtr(self->consoleWindow, GWL_STYLE,
		self->lOriginalParams & ~WS_SIZEBOX & ~WS_SYSMENU & ~WS_MINIMIZEBOX);	

	self->Height = height;
	self->Width = width;

	self->hOut = CreateConsoleScreenBuffer(
		GENERIC_READ |           // read/write access 
		GENERIC_WRITE,
		FILE_SHARE_READ |
		FILE_SHARE_WRITE,        // shared 
		NULL,                    // default security attributes 
		CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE 
		NULL);

	WND_CALL_INFO(self->hOut != INVALID_HANDLE_VALUE);
	WND_CALL_INFO(SetConsoleActiveScreenBuffer(self->hOut));
	WND_CALL_INFO(_SetFont(self, fontw, fonth));

	COORD coordLargest = GetLargestConsoleWindowSize(self->hOut);
	if (height > coordLargest.Y)
		self->Height = coordLargest.Y;
	if (width > coordLargest.X)
		self->Width =  coordLargest.X;
	coordLargest = (COORD) { self->Width, self->Height};

	// Set size of buffer
	WND_CALL_INFO(SetConsoleScreenBufferSize(self->hOut, coordLargest));

	// Set Physical Console Window Size
	self->rWindowRect = (SMALL_RECT){ 0, 0, (short)self->Width - 1, (short)self->Height - 1 };
	WND_CALL_INFO(SetConsoleWindowInfo(self->hOut, TRUE, &self->rWindowRect));

	CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
	GetConsoleScreenBufferInfo(self->hOut, &csbi);
	csbi.dwSize.Y--;
	SetConsoleScreenBufferSize(self->hOut, csbi.dwSize);// a little fix of scrollbars

	return coordLargest;
}

void _OutputToScreen(selfptr)
{
	WriteConsoleOutputW(
		self->hOut,
		*self->ppBuffer,
		(COORD) {(short)self->Width, (short)self->Height},
		(COORD) {0, 0},
		&self->rWindowRect);
}

void _SetPalette(selfptr, COLORREF palette[16])
{
	CONSOLE_SCREEN_BUFFER_INFOEX csbiex = { 0 };
	csbiex.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(self->hOut, &csbiex);				
	memcpy(csbiex.ColorTable, palette, 16 * sizeof(COLORREF));
	SetConsoleScreenBufferInfoEx(self->hOut, &csbiex);
}

VirtualTable{
	.CreateConsole = _CreateConsole,
	.SetCursor = _SetCursor,
	.Restore = _Restore,
	.OutputToScreen = _OutputToScreen,
	.SetPalette = _SetPalette
};

Constructor(selfptr, va_list* ap)
{
	assignMethodTable(self);
	self->consoleWindow = GetConsoleWindow();
	self->hIn = GetStdHandle(STD_INPUT_HANDLE);
	self->hOriginalConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	self->hOut = self->hOriginalConsole;
	return self;
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

const char* virtual(GetType)(void)
{
	return "Console Window Exception\n\r";
}
String TranslateErrorCode(HRESULT hr)
{
	String out;
	char* pMsgBuf = NULL;
	DWORD nMsgLen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), pMsgBuf,
		0, NULL);

	if (nMsgLen == 0)
	{
		return make_string("Unknown error");
	}
	out = make_string_l(pMsgBuf, nMsgLen);
	LocalFree(pMsgBuf);
	return out;
}
String GetErrorString(selfptr)
{
	return TranslateErrorCode(self->hr);
}
const char* virtual(what)(selfptr)
{
	String error = GetErrorString(self);
	String origin = self->method->GetOriginString(self);

	String out = string_fmt("%s\n%s[Error Code]: %lu\n[Description]: %s\n%s", virtual(GetType)(), self->hr, c_str(&error), c_str(&origin));
	string_move(&self->whatBuffer, &out);
	string_remove(&origin);
	string_remove(&error);

	return c_str(&self->whatBuffer);
}

Constructor(selfptr, va_list* ap)
{
	struct c_class *this = ((struct Class*)Exception)->ctor(self,ap);
	self->hr = va_arg(*ap, HRESULT);

	override(what);
	override(GetType);

	return this;
}
Destructor(selfptr)
{
	struct c_class *this = ((struct Class*)Exception)->dtor(self);
	return this;
}
ENDCLASSDESC 