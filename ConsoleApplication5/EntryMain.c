#include "Maze3D.h"
#include "CubeDemo.h"
#include "Exception.h"

int main(void)
{
	//struct Maze3D *Game;
	struct CubeDemo *Demo;
	try
	{
		Demo = new(CubeDemo);
		if (Demo->_base.method->SetupScreen(Demo, 320, 180, 4, 4))
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