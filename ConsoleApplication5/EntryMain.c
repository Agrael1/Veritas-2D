#include "Maze3D.h"
#include "CubeDemo.h"
#include "Exception.h"

#pragma warning(disable:4133)

int main(void)
{
	struct CubeDemo *Demo;
	try
	{
		Demo = new(CubeDemo);
		Demo->_base.method->SetupScreen(Demo, 640, 360, 2, 2);
		Demo->_base.method->Start(Demo);
	}
	catch (Exception, e)
	{
		MessageBoxA(NULL, e->method->what(e), e->method->GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	endtry

	if(Demo)
		delete(Demo);

	return 0;
}