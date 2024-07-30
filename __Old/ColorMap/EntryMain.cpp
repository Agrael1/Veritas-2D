#include "GDIPlusManager.h"
#include "Surface.h"
#include "../ConsoleApplication5/L16TC.palette"
#include <fstream>
#include <algorithm>

struct ColorMap
{
	COLORREF palette[16];
	DWORD paletteSZ;
	DWORD Height;
	DWORD Width;
	CHAR_INFO* map;
};

void BGRtoARGB(DWORD& in)
{
	BYTE* xin = (BYTE*)(&in);
	BYTE tmp = xin[0];
	xin[0] = xin[2];
	xin[2] = tmp;
}

ColorMap ConvertToCM(Surface& S, const COLORREF palette[16])
{
	COLORREF swapPalette[16];
	for (auto i = 0; i<16;i++)
	{
		swapPalette[i] = palette[i];
		BGRtoARGB(swapPalette[i]);
	}

	ColorMap CM;
	CM.paletteSZ = 16u;
	memcpy(CM.palette, palette, 16 * sizeof(COLORREF));
	CM.Height = S.GetHeight();
	CM.Width = S.GetWidth();

	DWORD* color = reinterpret_cast<DWORD*>(S.GetBufferPtr().Scan0);
	CM.map = new CHAR_INFO[CM.Width*CM.Height];

	for (UINT64 i = 0; i < CM.Width*CM.Height; i++)
	{
		CM.map[i].Char.UnicodeChar = L' ';
		CM.map[i].Attributes = 0ui16;
		for (UINT j = 0; j < 16; j++)
		{
			if ((color[i] & 0x00ffffff) == swapPalette[j])
			{
				CM.map[i].Attributes = j*0x10;	// alpha chanel not included
				break;
			}
		}
	}

	CM.Height--;
	CM.Width--;
	return CM;
}
void MakeCM(ColorMap& CM, std::string paletteName)
{
	std::ofstream CMOut;
	CMOut.open(L"..\\ConsoleApplication5\\Models\\Nano\\optiarm_dif.cm", std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
	if (!CMOut.is_open())
		return;

	CMOut.write(reinterpret_cast<const char*>(&CM.paletteSZ), sizeof(DWORD));
	CMOut << std::endl << paletteName << std::endl;

	CMOut.write(reinterpret_cast<const char*>(CM.palette), 16 * sizeof(COLORREF));
	CMOut << std::endl;
	CMOut.write(reinterpret_cast<const char*>(&CM.Width), sizeof(DWORD));
	CMOut.write(reinterpret_cast<const char*>(&CM.Height), sizeof(DWORD));
	CMOut << std::endl;
	CMOut.write(reinterpret_cast<const char*>(CM.map), (CM.Width+1) * (CM.Height+1) * sizeof(CHAR_INFO));
}

int __stdcall WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	GDIPlusManager manager;
	Surface S(L"..\\ConsoleApplication5\\Models\\Nano\\optiarm_dif_16.png");
	MakeCM(ConvertToCM(S, pPalette), pPaletteName);
	return 0;
}