#include <Maze.h>

int main()
{
	Maze maze = { 0 };
	Maze_ctor(&maze, (VConsoleDesc) { .fonth = 4, .fontw = 4, .height = 240, .width = 320 }, 319, 239);
	Start_Maze(&maze);

	Maze_dtor(&maze);
	return 0;
}
