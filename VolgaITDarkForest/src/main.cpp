#include "architecture/Maze.hpp"

int main()
{
	//architecture::WayTree* wayTree = new architecture::WayTree();

	//wayTree->addOrSetNodeInConcreteDirection(Direction::Up);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Up);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Up);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Right);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Right);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Right);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Right);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Down);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Down);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Down);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Left);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Left);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Up);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Up);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Down);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Down);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Left);
	//wayTree->addOrSetNodeInConcreteDirection(Direction::Left);

	//auto result = wayTree->findShortestWayToPositionFromCurrent(std::pair<int, int>(2,2));

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
