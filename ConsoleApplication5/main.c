#include "GameDemo.h"
#include "New.h"

main()
{
	struct GameDemo *GD = new(GameDemo);

	GD->_.ConstructConsole(GD, 160, 100, 8, 8);
	GD->_.Start(GD);
	delete(GD);
	return 0;
}

/*
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define M_PI 3.14159265358979323846

#define true 1
#define false 0

int nScreenWidth = 120;
int nScreenHeght = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = M_PI/4;

int fDepth = 16;



int main()
{
	WCHAR* screen = malloc(nScreenHeght*nScreenWidth*sizeof(WCHAR));
	void* hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	COORD nullc = { 0,0 };

	WCHAR* map = 
		L"################"
		L"#..............#"
		L"#..............#"
		L"#..............#"
		L"#..............#"
		L"##########.....#"
		L"#..............#"
		L"#..............#"
		L"#..............#"
		L"#..............#"
		L"#.........###..#"
		L"#.........#....#"
		L"#.........#....#"
		L"#.........######"
		L"#..............#"
		L"################";


	clock_t tp1, tp2;
	tp1 = clock(); 
	tp2 = clock(); 

	while (true)
	{

		tp2 = clock();
		clock_t elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = (float)(elapsedTime) / (float)(CLOCKS_PER_SEC);

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			fPlayerA -= (1.0f)*fElapsedTime;
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			fPlayerA += (1.0f)*fElapsedTime;
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			
			fPlayerX += sinf(fPlayerA)*(5.0f)*fElapsedTime;
			fPlayerY += cosf(fPlayerA)*(5.0f)*fElapsedTime;
			
			if (map[(int)fPlayerY*nMapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX -= sinf(fPlayerA)*(5.0f)*fElapsedTime;
				fPlayerY -= cosf(fPlayerA)*(5.0f)*fElapsedTime;
			}
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			fPlayerX -= sinf(fPlayerA)*(5.0f)*fElapsedTime;
			fPlayerY -= cosf(fPlayerA)*(5.0f)*fElapsedTime;
			
			if (map[(int)fPlayerY*nMapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX += sinf(fPlayerA)*(5.0f)*fElapsedTime;
				fPlayerY += cosf(fPlayerA)*(5.0f)*fElapsedTime;
			}
		}

		for (int x = 0; x < nScreenWidth; x++)
		{
			// Divides FOV by 2 and project a ray to World Space
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth)*fFOV;

			float fDistancetoWall = 0;
			char bHitWall = false;



			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall&&fDistancetoWall<fDepth)
			{
				fDistancetoWall += 0.1f;

				int nTestX = (int)(fPlayerX + fEyeX*fDistancetoWall);
				int nTestY = (int)(fPlayerY + fEyeY*fDistancetoWall);

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestX >= nMapHeight)
				{
					bHitWall = true;
					fDistancetoWall = fDepth;
				}
				else
				{
					//Raytest
					if (map[nTestY*nMapWidth + nTestX] == '#')
					{
						bHitWall = true;
					}
				}
			}
			//Calculate perspective
			int nCeiling = (float)(nScreenHeght / 2.0) - nScreenHeght / ((float)fDistancetoWall);
			int nFloor = nScreenHeght - nCeiling;

			short nShade = ' ';
			short nFloorShade = ' ';

			if (fDistancetoWall <= fDepth / 4.0f)		nShade = 0x2588;
			else if (fDistancetoWall <= fDepth / 3.0f)	nShade = 0x2593;
			else if (fDistancetoWall <= fDepth / 2.0f)	nShade = 0x2592;
			else if (fDistancetoWall <= fDepth)			nShade = 0x2591;
			else										nShade = ' ';

			for (int y = 0; y < nScreenHeght; y++)
			{
				if (y < nCeiling)
				{
					screen[y*nScreenWidth + x] = ' ';
				}
				else if (y > nCeiling&&y <= nFloor)
				{
					screen[y*nScreenWidth + x] = nShade;
				}
				else
				{
					// Shade floor
					float b = 1.0f - (((float)y - nScreenHeght / 2.0f) / ((float)nScreenHeght / 2.0f));
					if (b < 0.25)			nFloorShade = '#';
					else if (b < 0.5)		nFloorShade = 'x';
					else if (b < 0.75)		nFloorShade = '.';
					else if (b < 0.9)		nFloorShade = '-';
					else					nFloorShade = ' ';
					screen[y*nScreenWidth + x] = nFloorShade;
				}
			}
		}
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, (1.0f) / fElapsedTime);

		// Display map
		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapWidth; ny++)
				screen[(ny + 1)*nScreenWidth + nx] = map[ny*nMapWidth + nx];

		screen[((int)fPlayerY + 1)*nScreenWidth + (int)fPlayerX] = 'P';

		*(screen + nScreenHeght*nScreenWidth - 1) = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenHeght*nScreenWidth, nullc, &dwBytesWritten);
	}
	return 0;
}
*/