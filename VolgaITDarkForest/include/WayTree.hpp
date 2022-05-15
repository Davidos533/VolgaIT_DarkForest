#ifndef WAY_TREE_HPP
#define WAY_TREE_HPP

#include "WayNode.hpp"

namespace maze
{
	class WayTree
	{
	private:
		WayNode* currentPosition;
	public:
		WayTree();

		//WayTree() = delete;
		WayTree(const WayTree& object) = delete;

		// method to add new way node to current position
		bool addNode(Direction direction);
		WayNode* findNodeByCoordiantes(std::pair<int, int> coordiantes, WayNode* node = nullptr);
		void updateAllCoordiantes(int up, int down, int left, int right, WayNode* node = nullptr);

		bool removeNode(WayNode* wayNode);

		~WayTree();
	};
}

#endif