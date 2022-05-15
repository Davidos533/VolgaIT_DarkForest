#ifndef WAY_NODE_HPP
#define WAY_NODE_HPP

#include <iostream>

#include "fairy_tail.hpp"

class WayNode
{
public:
	std::pair<int, int> coordinates;
	Direction direction;

	WayNode* UpNode = nullptr;
	WayNode* DownNode = nullptr;
	WayNode* LeftNode = nullptr;
	WayNode* RightNode = nullptr;
};

#endif