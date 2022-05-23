#include "architecture/Maze.hpp"

#include <cstdio>

void freeMazeResources(architecture::Maze* maze);

int main(int argc, const char* argv[])
{
	std::cerr << "place file with maze data near:\n" << argv[0] <<"\nand press any key...\n";

	getchar();

	architecture::Maze* maze = nullptr;

	try
	{
		maze = new architecture::Maze();

		maze->startSearchingMeetPath();
	}
	catch (std::runtime_error error)
	{
		std::cerr << "program fatal error =(" << "\n";
	}
	
	freeMazeResources(maze);

	getchar();

	return 0;
}

void freeMazeResources(architecture::Maze* maze)
{
	if (maze != nullptr)
	{
		delete maze;
	}
}