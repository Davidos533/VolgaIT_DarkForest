#include "architecture/Maze.hpp"

#include <vld.h>

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
	
	return 0;
}
