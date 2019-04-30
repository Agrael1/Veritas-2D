#include "Maze3D.h"
#include "Exception.h"

int main(void)
{
	struct Maze3D *Game;
	try
	{
		Game = new(Maze3D);
		if (Game->_base.method->SetupScreen(Game, 320, 160, 4, 4))
			Game->_base.method->Start(Game);
	}
	catch (Exception, e)
	{
		MessageBoxA(NULL, e->method->what(e), e->method->GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	endtry

	if(Game)
		delete(Game);

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