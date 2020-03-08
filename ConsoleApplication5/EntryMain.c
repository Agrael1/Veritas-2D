#include "Maze3D.h"
#include "CubeDemo.h"
#include "Exception.h"

#pragma warning(disable:4133)


int main(void)
{
	struct CubeDemo Demo;
	struct Maze3D Maze;
	try
	{
		construct(&Maze, Maze3D);
		Maze.method->Start(&Maze);
	}
	catch (Exception, e)
	{
		MessageBoxA(NULL, e->method->what(e), e->method->GetType(), MB_OK | MB_ICONEXCLAMATION);
		delete(e);
		return 1;
	}
	endtry;

	deconstruct(&Maze);
	return 0;
}