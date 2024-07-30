#include "CubeDemo.h"
#include <Exception.h>

#pragma warning(disable:4133)
int main(void)
{
	FunctionPrologue;

	struct CubeDemo Demo;
	try
	{
		construct(&Demo, CubeDemo);
		Demo.method->Start(&Demo);
	}
	catch (Exception, e)
	{
		MessageBoxA(NULL, e->method->what(e), e->method->GetType(), MB_OK | MB_ICONEXCLAMATION);
		delete(e);
		Return 1;
	}
	endtry;

	Return 0;
}