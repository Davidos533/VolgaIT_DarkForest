#ifndef WAY_NODE_HPP
#define WAY_NODE_HPP

#include <iostream>

#include "common/api/fairy_tail.hpp"

namespace architecture
{

	class WayNode
	{
	public:
		std::pair<int, int> coordinates;
		bool isBarrier = false;
		WayNode* UpNode = nullptr;
		WayNode* DownNode = nullptr;
		WayNode* LeftNode = nullptr;
		WayNode* RightNode = nullptr;
	};
}

#endif