#ifndef WAY_NODE_HPP
#define WAY_NODE_HPP

#include <iostream>

#include "common/api/fairy_tail.hpp"

namespace architecture
{
	using Position = std::pair<int, int>;
	class WayNode
	{
	public:
		Position coordinates;
		bool isBarrier = false;
		WayNode* UpNode = nullptr;
		WayNode* DownNode = nullptr;
		WayNode* LeftNode = nullptr;
		WayNode* RightNode = nullptr;
	};
}

#endif