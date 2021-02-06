#include "../../MediaPlayer/MPAPI.h"
#include <VeritasEngine.h>

#pragma comment(lib, "C:\\Users\\aa\\Documents\\Visual Studio 2019\\Projects\\Veritas-3D\\x64\\Release\\MediaPlayer.lib")

enum DEF_COLOUR
{
	DFG_BLACK = 0x0000,
	DFG_DARK_BLUE = 0x0001,
	DFG_DARK_GREEN = 0x0002,
	DFG_DARK_CYAN = 0x0003,
	DFG_DARK_RED = 0x0004,
	DFG_DARK_MAGENTA = 0x0005,
	DFG_DARK_YELLOW = 0x0006,
	DFG_GREY = 0x0007,
	DFG_DARK_GREY = 0x0008,
	DFG_BLUE = 0x0009,
	DFG_GREEN = 0x000A,
	DFG_CYAN = 0x000B,
	DFG_RED = 0x000C,
	DFG_MAGENTA = 0x000D,
	DFG_YELLOW = 0x000E,
	DFG_WHITE = 0x000F,

	DBG_BLACK = 0x0000,
	DBG_DARK_BLUE = 0x0010,
	DBG_DARK_GREEN = 0x0020,
	DBG_DARK_CYAN = 0x0030,
	DBG_DARK_RED = 0x0040,
	DBG_DARK_MAGENTA = 0x0050,
	DBG_DARK_YELLOW = 0x0060,
	DBG_GREY = 0x0070,
	DBG_DARK_GREY = 0x0080,
	DBG_BLUE = 0x0090,
	DBG_GREEN = 0x00A0,
	DBG_CYAN = 0x00B0,
	DBG_RED = 0x00C0,
	DBG_MAGENTA = 0x00D0,
	DBG_YELLOW = 0x00E0,
	DBG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
	PIXEL_NONE = ' '
};

void* player;
uint32_t width = 0, height = 0;

bool OnUserUpdate(VeritasEngine* self, float unused)
{
    if (!MPIsFinished(player))
    {
        VideoSlice x = MPGetFrame(player);
        if (x.RowPitch)
        {
            for (size_t i = 0; i < height; i++)
            {
                for (size_t j = 0; j < width; j++)
                {
                    CHAR_INFO out = {.Char = PIXEL_NONE, .Attributes = DBG_BLACK };
					uint32_t val = x.pData[i * width + j] & 0x00ffffff;

					if (val >= 0x00BDBDBD)
						out.Attributes = DBG_WHITE;
					else if (val <= 0x00BDBDBD && val >= 0x007f7f7f)
						out.Attributes = DBG_GREY;
					else if (val <= 0x007f7f7f && val >= 0x003f3f3f)
						out.Attributes = DBG_DARK_GREY;
                    PrintFrame(&self->Swap, j,i,out );
                }
            }
            return true;
        }
    }
    return true;
}
bool OnUserCreate(VeritasEngine* self)
{
	return true;
}

int main(void)
{
    player = MPInitialize(); 
    MPSetVideoResource(player, L"C:/users/aa/desktop/BadApple.mp4");

    if (MPIsInfoReady(player))
    {
        MPGetVideoSize(player, &width, &height);

        VeritasEngine ve;
        VeritasEngine_ctor(&ve, (VConsoleDesc){ .width = width, .height = height, .fontw = 2, .fonth = 2 });
		
		ve.method->OnUserCreate = OnUserCreate;
		ve.method->OnUserUpdate = OnUserUpdate;

		SetConsoleTitle(TEXT("Veritas Bad Apple"));

        MPPlay(player);

        Start(&ve); // Start the rave!

        VeritasEngine_dtor(&ve);
    }

    MPRelease(player);
    return 0;
}