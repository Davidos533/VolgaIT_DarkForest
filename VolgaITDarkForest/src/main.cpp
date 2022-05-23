#include "architecture/Maze.hpp"

#include <cstdio>

int main()
{
	architecture::Maze* maze = new architecture::Maze();

	try
	{
		maze->startSearchingMeetPath();
	}
	catch (std::runtime_error error)
	{
		std::cerr << error.what() << "\n";
	}
	
	delete maze;
	
	getchar();

	return 0;
}
