#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <vector>

#include "fairy_tail.hpp"
#include "Person.hpp"
#include "WayTree.hpp"

int walk();

int main()
{
    maze::WayTree* wayTree = new maze::WayTree();

    wayTree->addNode(Direction::Up);
    wayTree->addNode(Direction::Left);


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

        maze::Person king(world, Character::Ivan);
        maze::Person queen(world, Character::Elena);

        // persons sides walk lengths
        std::vector<int> sidesWalkLengtKing(4);
        std::vector<int> sidesWalkLengthQueen(4);

        do
        {
            king.moove();
            queen.moove();
        } while (!world.go(king.getDirection(), queen.getDirection()));

    }
    catch (std::runtime_error)
    {
    
    }
    return world.getTurnCount();
}