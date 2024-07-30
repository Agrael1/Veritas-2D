#define WINDOW_IMPL
#include <engine/window.h>
#include <malloc.h>

extern inline void Constructor(selfptr);
extern inline void Destructor(selfptr);

extern inline bool ConShowCursor(selfptr, bool value);
extern inline void Restore(selfptr);
extern inline void SetPalette(selfptr, COLORREF palette[16]);
extern inline void OutputToScreen(selfptr, CHAR_INFO* buffer);

bool SetFont(const selfptr, uint8_t fontw, uint8_t fonth)
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
COORD CreateConsole(selfptr, uint16_t width, uint16_t height, uint8_t fontw, uint8_t fonth)
{
	if (self->hOut != self->hOriginalConsole)
		return self->Dimensions;

	self->lOriginalParams = GetWindowLong(self->ConsoleWindow, GWL_STYLE);
	SetWindowLongPtr(self->ConsoleWindow, GWL_STYLE,
		self->lOriginalParams & ~WS_SIZEBOX & ~WS_SYSMENU & ~WS_MINIMIZEBOX);


	self->Dimensions = (COORD){ .Y = height, .X = width };

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
	WND_CALL_INFO(SetFont(self, fontw, fonth));

	COORD coordLargest = GetLargestConsoleWindowSize(self->hOut);
	if (height > coordLargest.Y)
		self->Dimensions.Y = coordLargest.Y;
	if (width > coordLargest.X)
		self->Dimensions.X = coordLargest.X;
	coordLargest = self->Dimensions;

	// Set size of buffer
	WND_CALL_INFO(SetConsoleScreenBufferSize(self->hOut, coordLargest));

	// Set Physical Console Window Size
	self->rWindowRect = (SMALL_RECT){ 0, 0, self->Dimensions.X - 1, self->Dimensions.Y - 1 };
	WND_CALL_INFO(SetConsoleWindowInfo(self->hOut, TRUE, &self->rWindowRect));

	CONSOLE_SCREEN_BUFFER_INFO csbi = { 0 };
	GetConsoleScreenBufferInfo(self->hOut, &csbi);
	csbi.dwSize.Y--;
	SetConsoleScreenBufferSize(self->hOut, csbi.dwSize);// a little fix of scrollbars

	return coordLargest;
}
bool CatchFocus(selfptr)
{
	DWORD numRecords;
	DWORD numRecsRecieved;
	GetNumberOfConsoleInputEvents(self->hIn, &numRecords);
	PINPUT_RECORD records = malloc(numRecords * sizeof(INPUT_RECORD));
	ReadConsoleInput(self->hIn, records, numRecords, &numRecsRecieved);

	bool out = false;

	for (DWORD i = 0; i < numRecsRecieved; i++)
	{
		if (records[i].EventType == FOCUS_EVENT)
		{
			out = records[i].Event.FocusEvent.bSetFocus;
		}
	}
	free(records);
	return out;
}
