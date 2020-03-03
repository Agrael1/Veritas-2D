#include "Maze3D.h"
#include "CubeDemo.h"
#include "Exception.h"
#include <stdio.h>

#pragma warning(disable:4133)

int main(void)
{
	String s = make_string("hello");
	char c[128];
	sprintf(c,"ggwp %d\n", string_length(&s));

	struct CubeDemo Demo;
	construct(&Demo, CubeDemo);

	try
	{
		Demo._base.method->SetupScreen(&Demo, 640, 360, 2, 2);
		Demo._base.method->Start(&Demo);
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