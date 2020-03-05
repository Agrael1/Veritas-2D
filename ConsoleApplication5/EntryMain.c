#include "Maze3D.h"
#include "CubeDemo.h"
#include "Exception.h"

#pragma warning(disable:4133)


int main(void)
{
	struct CubeDemo Demo;
	construct(&Demo, CubeDemo);

	try
	{
		Demo.method->SetupScreen(&Demo, 640, 360, 2, 2);
		Demo.method->Start(&Demo);
	}
	catch (Exception, e)
	{
		MessageBoxA(NULL, e->method->what(e), e->method->GetType(), MB_OK | MB_ICONEXCLAMATION);
		delete(e);
		return 1;
	}
	endtry;

	deconstruct(&Demo);
	return 0;
}