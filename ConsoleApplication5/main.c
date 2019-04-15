#include <stdio.h>
#include <malloc.h>
//#include "GameDemo.h"
//#include "VeritasEngine.h"
#include "Exception.h"
#include <setjmpex.h>

jmp_buf env;
_declspec(thread) volatile struct Exception *e;

int v(void)
{
	int a = 88;
	if (a == 88)
	{	
		e = new(Exception, __LINE__, __FILE__);
		longjmp(env, 1);
	}
	return a;
}

main()
{
	/*struct GameDemo *GD = new(GameDemo);

	GD->_.method->ConstructConsole(GD, 320, 200, 4, 4);
	GD->_.method->Start(GD);
	
	delete(GD);*/
	
	if (setjmp(env))
	{
		e->method->what(e);
		printf("%s",e->whatBuffer );
	}
	else
		v();
	
	getchar();
	/*struct VeritasEngine *VE = new(VeritasEngine);
	if(VE->method->ConstructConsole(VE, 1000, 1000, 8, 8))
		VE->method->Start(VE);*/

	/*delete(VE);*/

}