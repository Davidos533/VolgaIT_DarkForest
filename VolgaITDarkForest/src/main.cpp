#include "architecture/Maze.hpp"

int main()
{
	architecture::Maze maze;

	try
	{
		maze.startSearchingMeetPath();
	}
	catch (std::runtime_error error)
	{
		std::cerr << error.what() << "\n";
	}

	return 0;
}
