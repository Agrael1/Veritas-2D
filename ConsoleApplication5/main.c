#include "BitStack.h"
#include <stdio.h>
#include "GameDemo.h"
#include "VeritasEngine.h"



main()
{
	struct GameDemo *GD = new(GameDemo);

	GD->_.method->ConstructConsole(GD, 320, 200, 4, 4);
	GD->_.method->Start(GD);

	delete(GD);

	/*struct VeritasEngine *VE = new(VeritasEngine);
	if(VE->method->ConstructConsole(VE, 1000, 1000, 8, 8))
		VE->method->Start(VE);*/

	/*delete(VE);*/

}