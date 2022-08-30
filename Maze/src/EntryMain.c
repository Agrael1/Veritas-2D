#include <Maze.h>

int main()
{
	Maze maze = {0};
	Maze_ctor(&maze, (VConsoleDesc) { .fonth = 2, .fontw = 2, .height = 480, .width = 640 });
	Start_Maze(&maze);
	
	Maze_dtor(&maze);
	return 0;
}
