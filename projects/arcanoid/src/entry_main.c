#include <game/game.h>


int main()
{
    Game game;
    Game_ctor(&game, (VConsoleDesc){ 300, 300, 4, 4 });

    int ret = ArkanoidStart(&game);

    Game_dtor(&game);
    return ret;
}