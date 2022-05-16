#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <vector>

#include "common/api/fairy_tail.hpp"
#include "architecture/Person.hpp"
#include "architecture/WayTree.hpp"

int walk();

int main()
{
	architecture::WayTree* wayTree = new architecture::WayTree();

	wayTree->addNode(Direction::Right);
	wayTree->addNode(Direction::Left);
	wayTree->addNode(Direction::Up);
	wayTree->addNode(Direction::Right);
	wayTree->addNode(Direction::Right);
	wayTree->addNode(Direction::Right);
	wayTree->addNode(Direction::Left);
	wayTree->addNode(Direction::Down);
	wayTree->addNode(Direction::Down);
	wayTree->addNode(Direction::Right);
	wayTree->addNode(Direction::Left);
	wayTree->addNode(Direction::Left);
	wayTree->addNode(Direction::Left);
	wayTree->addNode(Direction::Up);

	architecture::WayNode* result = wayTree->findNodeByCoordiantes(std::pair<int, int>(1, 0));

	if (result != nullptr)
	{
		std::cerr << "Node found: x:" << result->coordinates.first << "\ty:" << result->coordinates.second<<"\n";
	}
	else
	{
		std::cerr << "Warning! Node not found";
	}

	delete wayTree;

	//srand(static_cast<unsigned int>(time(nullptr)));
	////CircularList<int> a;
	//if (const int turns = walk())
	//    std::cout << "Found in " << turns << " turns" << std::endl;
	//else
	//    std::cout << "Not found" << std::endl;
	//
	//_getch();

	return 0;
}


// method to make a meeting
int walk()
{
	Fairyland world;
	try
	{
		long iterNums;

		//architecture::Person king(world, Character::Ivan);
		//architecture::Person queen(world, Character::Elena);

		// persons sides walk lengths
		//std::vector<int> sidesWalkLengtKing(4);
		//std::vector<int> sidesWalkLengthQueen(4);

		//do
		//{
		//	king.moove();
		//	queen.moove();
		//} while (!world.go(king.getDirection(), queen.getDirection()));

	}
	catch (std::runtime_error)
	{

	}
	return world.getTurnCount();
}