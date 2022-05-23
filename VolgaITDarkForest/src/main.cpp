#include "architecture/Maze.hpp"

#include <cstdio>

int main(int argc, const char* argv[])
{
	std::cerr << "place file with maze data near:\n" << argv[0] <<"\nand press any key...\n";

	getchar();

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
