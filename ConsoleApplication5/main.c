#include <stdio.h>
#include "GameDemo.h"
#include "VeritasEngine.h"
#include "Exception.h"
#include "Queue.h"

int main(void)
{
	//int x = 123;
	//int* ff = &x;
	//struct Queue* q = new(Queue, sizeof(int*), 2);

	//q->method->push(q, x);
	//q->method->push(q, x+2);
	//q->method->push(q, x + 10);

	//q->method->pop(q, &x);

	//printf("popped: %d",x);

	//q->method->clear(q);

	//q->method->push(q, x + 10);

	//q->method->pop(q, &x);
	//printf("popped: %d", x );

	struct VeritasEngine *VE;
	try
	{
		VE = new(VeritasEngine);
		if (VE->method->SetupScreen(VE, 320, 160, 8, 8))
			VE->method->Start(VE);

	}
	catch (Exception, e)
	{
		MessageBoxA(NULL, e->method->what(e), e->method->GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	endtry

	if(VE)
		delete(VE);

	return 0;


	
	/*struct GameDemo *GD;
	try 
	{
		GD = new(GameDemo);

		if(GD->_.method->ConstructConsole(GD, 320, 200, 4, 4))
			GD->_.method->Start(GD);
	}
	catch (Exception, e)
	{
		e->method->what(e);
		printf("%s", e->whatBuffer);
	}
	endtry

	if (GD)
		delete(GD);*/

}