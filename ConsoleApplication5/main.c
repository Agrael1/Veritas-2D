#include "GameDemo.h"
#include "New.h"
#include "BitStack.h"
#include <stdio.h>

main()
{
	struct GameDemo *GD = new(GameDemo);

	GD->_.method->ConstructConsole(GD, 160, 100, 8, 8);
	GD->_.method->Start(GD);

	delete(GD);
}